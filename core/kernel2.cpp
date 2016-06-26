#include "kernel2.h"
#include "core/lzs.h"

struct Kernel2TocInfos {
	explicit Kernel2TocInfos(int position = 0, int padding = 0)
	    : position(position), padding(padding)
	{
	}
	int position, padding;
};

Kernel2::Kernel2() : _uncompressedDataSize(-1)
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
	const int dataSize = data.size();

	if (dataSize < 4) {
		setError(TooShortError);
		return false;
	}

	QByteArray decData = LZS::decompressAll(data.constData() + 4, dataSize - 4);

	if (decData.isEmpty()) {
		setError(LzsError);
		return false;
	}

	const char *constData = decData.constData();
	_uncompressedDataSize = decData.size();

	_sections.clear();

	int pos = 0;

	forever {
		// End of file
		if (pos == _uncompressedDataSize) {
			break;
		}

		if (_uncompressedDataSize < pos + 4) {
			setError(TooShortError);
			return false;
		}

		int sectionSize;

		memcpy(&sectionSize, constData + pos, 4);

		pos += 4;

		if (_uncompressedDataSize < pos + sectionSize) {
			setError(SectionHeaderError);
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

		_sections.append(names);

		pos += sectionSize;
	}

	setError(Ok);

	return true;
}

bool Kernel2::save(const QString &path, bool sharedData,
                   bool doNotBreakFileFormat) const
{
	QByteArray data;
	if (!save(data, sharedData, doNotBreakFileFormat)) {
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
                          bool sharedData, bool doNotBreakFileFormat) const
{
	QList<Kernel2TocInfos> toc;
	QByteArray sectionData;
	int textID = 0;
	const int posDataStart = texts.size() * 2;

	foreach (const QByteArray &text, texts) {
		bool optimized = false;

		if (sharedData) {
			for (int i = 0; i < texts.size(); i += 1) {
				if (i == textID) { // Every other texts
					continue;
				}
				const QByteArray &text2 = texts.at(i);
				if (text2.endsWith(text)) {
					Kernel2TocInfos tocInfos;
					int diffSize = text2.size() - text.size();

					qDebug() << "Kernel2::saveSection" << textID
					         << "text endsWith" << FF7Text(text2).text(false)
					         << text2.mid(diffSize).toHex()
					         << FF7Text(text).text(false) << text.toHex();

					if (i < toc.size()) { // Update from existing toc entry
						const Kernel2TocInfos &existingInfos = toc.at(i);
						if (toc.at(i).padding == 0) {
							// Direct position
							tocInfos.position =
							    existingInfos.position + diffSize;
						} else {
							// Update padding
							tocInfos.position = existingInfos.position;
							tocInfos.padding = existingInfos.padding + diffSize;
						}

						qDebug() << "Kernel2::saveSection"
						         << "update existing toc entry" << i
						         << diffSize;
					} else if (!doNotBreakFileFormat && diffSize != 0) {
						/* If texts are equals, it will be merged later
						 * This will break the file format by reordering the toc
						 */
						tocInfos.position = i;
						tocInfos.padding = diffSize;

						qDebug() << "Kernel2::saveSection"
						         << "create special toc entry"
						         << tocInfos.position << tocInfos.padding;
					} else {
						continue;
					}
					toc.append(tocInfos);
					optimized = true;
					break;
				}
			}
		} else if (textID > 0 && text == "\xFF") {
			toc.append(Kernel2TocInfos(posDataStart + sectionData.size() - 1));
			optimized = true;
		}

		if (!optimized) {
			toc.append(Kernel2TocInfos(posDataStart + sectionData.size()));
			sectionData.append(text);
		}

		textID += 1;
	}

	QByteArray sectionToc;
	textID = 0;
	foreach (const Kernel2TocInfos &infos, toc) {
		int bigPos;
		if (infos.padding != 0) {
			const Kernel2TocInfos &pointedInfos = toc.at(infos.position);
			if (pointedInfos.padding != 0) {
				qWarning() << "Kernel2::saveSection error"
				           << "multiple infos recursions";
				return false; // FIXME: handle this error
			}
			bigPos = pointedInfos.position + infos.padding;
			toc[textID].position = bigPos;
		} else {
			bigPos = infos.position;
		}
		// Check if cast is safe
		if (bigPos > 65535) {
			qWarning() << "Kernel2::saveSection error"
			           << "position overflow";
			return false;
		}
		quint16 pos = quint16(bigPos);
		sectionToc.append(reinterpret_cast<char *>(&pos), 2);
		++textID;
	}

	qint32 sectionSize = sectionToc.size() + sectionData.size();

	data.append(reinterpret_cast<char *>(&sectionSize), 4)
	    .append(sectionToc)
	    .append(sectionData);

	return true;
}

bool Kernel2::saveUncompressed(QByteArray &data, bool sharedData,
                               bool doNotBreakFileFormat) const
{
	foreach (const QList<QByteArray> &texts, _sections) {
		saveSection(texts, data, sharedData, doNotBreakFileFormat);
	}

	return true;
}

bool Kernel2::save(QByteArray &data, bool sharedData,
                   bool doNotBreakFileFormat) const
{
	QByteArray decData;

	saveUncompressed(decData, sharedData, doNotBreakFileFormat);

	data = LZS::compress(decData);
	qint32 lzsSize = data.size();
	data.prepend(reinterpret_cast<char *>(&lzsSize), 4);

	return true;
}

bool Kernel2::extractSection(const QList<QByteArray> &texts,
                             const QString &path) const
{
	QByteArray data;
	QFile f(path);
	if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		saveSection(texts, data, false, true);
		f.write(data.constData() + 4, data.size() - 4);
		return true;
	}
	return false;
}

bool Kernel2::extractAll(const QString &dirPath, const QString &filename) const
{
	QDir dir(dirPath);
	int sectionID = 9;

	foreach (const QList<QByteArray> &texts, _sections) {
		if (!extractSection(texts, dir.filePath(filename.arg(sectionID)))) {
			return false;
		}
		++sectionID;
	}
	return true;
}

QString Kernel2::errorString() const
{
	switch (_error) {
	case Ok:
		return "OK";
	case TooShortError:
		return "Too short error";
	case LzsError:
		return "LZS compression error";
	case SectionHeaderError:
		return "Section header error";
	}
	return QString();
}
