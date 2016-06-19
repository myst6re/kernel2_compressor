#include "kernel2.h"
#include "core/lzs.h"

Kernel2::Kernel2() :
    _uncompressedDataSize(-1)
{
}

bool Kernel2::open(const QString &path)
{
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) {
		return false;
	}

	return open(f.readAll());
}

bool Kernel2::open(const QByteArray &data)
{
	int lzsSize;
	const int dataSize = data.size();

	if (dataSize < 4) {
		return false;
	}

	memcpy(&lzsSize, data.constData(), 4); // LIMITATION: signed int

	if(lzsSize + 4 != dataSize) {
		return false;
	}

	QByteArray decData = LZS::decompressAll(data.constData() + 4, lzsSize);
	const char *constData = decData.constData();
	_uncompressedDataSize = decData.size();

	sections.clear();

	int pos = 0;

	forever {
		// End of file
		if (pos == _uncompressedDataSize) {
			break;
		}

		if (_uncompressedDataSize < pos + 4) {
			return false;
		}

		int sectionSize;

		memcpy(&sectionSize, constData + pos, 4);

		pos += 4;

		if (_uncompressedDataSize < pos + sectionSize) {
			return false;
		}

		QList<QByteArray> names;
		int minPosition = sectionSize;

		for (int i = 0; i < minPosition / 2; i += 1) {
			quint16 position;
			memcpy(&position, constData + pos + i * 2, 2);
			if (position >= sectionSize) {
				break;
			}
			if (position < minPosition) {
				minPosition = position;
			}

			int index = FF7Text::indexOfFF(decData, pos + position);
			if (index != -1) {
				names.append(decData.mid(
				            pos + position,
				            qMin((index + 1) - (pos + position), sectionSize)));
			} else {
				names.append(decData.mid(pos + position, sectionSize));
			}
		}

		sections.append(names);

		pos += sectionSize;
	}

	return true;
}

bool Kernel2::checkIntegrity() const
{
	// TODO
	return false;
}

bool Kernel2::save(const QString &path, bool sharedData) const
{
	QByteArray data;
	if (!save(data, sharedData)) {
		return false;
	}

	QFile f(path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		return false;
	}
	f.write(data);
	f.close();

	return true;
}

bool Kernel2::saveSection(const QList<QByteArray> &texts, QByteArray &data,
                          bool sharedData) const
{
	QList<quint32> toc;
	QByteArray sectionData;
	int textID = 0;
	const int posDataStart = texts.size() * 2;

	foreach (const QByteArray &text, texts) {
		bool optimized = false;

		if (sharedData) {
			for (int i = 0; i < texts.size(); ++i) {
				if (i == textID) {
					continue;
				}
				const QByteArray &text2 = texts.at(i);
				if (text2.endsWith(text)) {
					if (i < toc.size()) {
						if((toc.at(i) & 0x80000000) == 0) {
							toc.append(toc.at(i)
							           + (text2.size() - text.size()));
						} else {
							int padd = (toc.at(i) >> 16) & 0x7FFF;
							qDebug() << (toc.at(i) & 0xFFFF)
							         << padd
							         << (padd
							             + (text2.size() - text.size()));
							toc.append(int(toc.at(i) & 0xFFFF) |
							           (((padd + (text2.size()
							                      - text.size()))
							             & 0x7FFF) << 16) | 0x80000000);
						}
						qDebug() << textID << "text endsWith"
						         << FF7Text(text2).text(false)
						         << text2.mid(text2.size() - text.size())
						            .toHex()
						         << FF7Text(text).text(false)
						         << text.toHex() << "before"
						         << i << (text2.size() - text.size());
					} else if (text != text2) {
						qDebug() << textID << "text endsWith"
						         << FF7Text(text2).text(false)
						         << text2.mid(text2.size() - text.size())
						            .toHex() << FF7Text(text).text(false)
						         << text.toHex() << "after" << i
						         << (text2.size() - text.size());
						toc.append((i & 0xFFFF) |
						           (((text2.size() - text.size()) & 0x7FFF)
						            << 16) | 0x80000000);
					} else {
						continue;
					}
					optimized = true;
					break;
				}
			}
		}

		if(!optimized) {
			toc.append(posDataStart + sectionData.size());
			sectionData.append(text);
		}

		++textID;
	}

	QByteArray sectionToc;
	textID = 0;
	foreach (quint32 pos, toc) {
		if ((pos & 0x80000000) != 0) {
			quint16 padd = (pos >> 16) & 0x7FFF;
			pos = pos & 0xFFFF;
			qDebug() << "pos < 0" << pos << padd;
			if ((toc.at(pos) & 0x80000000) != 0) {
				qWarning() << "error";
			} else {
				qDebug() << toc.at(pos);
			}
			toc[textID] = toc.at(pos) + padd;
			pos = toc.at(pos) + padd;
		}
		sectionToc.append((char *)&pos, 2);
		++textID;
	}

	int sectionSize = sectionToc.size() + sectionData.size();

	data.append((char *)&sectionSize, 4)
			.append(sectionToc)
			.append(sectionData);

	return true;
}

bool Kernel2::saveUncompressed(QByteArray &data, bool sharedData) const
{
	foreach (const QList<QByteArray> &texts, sections) {
		saveSection(texts, data, sharedData);
	}

	return true;
}

bool Kernel2::save(QByteArray &data, bool sharedData) const
{
	QByteArray decData;
	int lzsSize;

	saveUncompressed(decData, sharedData);

	data = LZS::compress(decData);
	lzsSize = data.size();
	data.prepend((char *)&lzsSize, 4);

	return true;
}

bool Kernel2::extractSection(const QList<QByteArray> &texts,
                             const QString &path) const
{
	QByteArray data;
	QFile f(path);
	if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		saveSection(texts, data, false);
		f.write(data.constData() + 4, data.size() - 4);
		return true;
	}
	return false;
}

bool Kernel2::extractAll(const QString &dirPath, const QString &filename) const
{
	QDir dir(dirPath);
	int sectionID = 9;

	foreach (const QList<QByteArray> &texts, sections) {
		if (!extractSection(texts, dir.filePath(filename.arg(sectionID)))) {
			return false;
		}
		++sectionID;
	}
	return true;
}
