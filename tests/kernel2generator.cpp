#include "kernel2generator.h"
#include "core/lzs.h"

QByteArray Kernel2Generator::emptyText(int size)
{
	return QByteArray(size, '\0').append('\xff');
}

QList<QByteArray> Kernel2Generator::emptyTexts(int count, int size)
{
	QList<QByteArray> ret;

	for (int i = 0; i < count; i += 1) {
		ret.append(emptyText(size));
	}

	return ret;
}

QByteArray Kernel2Generator::randomSafeText(int size)
{
	QByteArray ret;
	ret.reserve(size + 1);

	for (int i = 0; i < size; i += 1) {
		ret.append(char(qrand() % 0xE9));
	}

	ret.append('\xff');

	return ret;
}

QList<QByteArray> Kernel2Generator::randomSafeTexts(int count, int size)
{
	QList<QByteArray> ret;

	for (int i = 0; i < count; i += 1) {
		ret.append(randomSafeText(size));
	}

	return ret;
}

QByteArray Kernel2Generator::section(const QList<QByteArray> &texts,
                         const QMap<int, Kernel2GeneratorTocAction> &tocActions)
{
	Q_UNUSED(tocActions); // TODO
	QByteArray realToc, ret;
	int tocSize = texts.size() * int(sizeof(quint16));
	QList<int> toc;

	foreach (const QByteArray &text, texts) {
		toc.append(tocSize + ret.size());
		ret.append(text);
	}

	foreach (int position, toc) {
		Q_ASSERT(position < 65536);
		quint16 pos = quint16(position);
		realToc.append(reinterpret_cast<char *>(&pos), 2);
	}

	ret.prepend(realToc);
	qint32 sectionSize = ret.size();

	return ret.prepend(reinterpret_cast<char *>(&sectionSize), 4);
}

QByteArray Kernel2Generator::sections(int count, int textCount)
{
	QByteArray ret;

	for (int i = 0; i < count; i += 1) {
		ret.append(section(randomSafeTexts(textCount, 16)));
	}

	return ret;
}

QByteArray Kernel2Generator::generate(const QByteArray &sections)
{
	return LZS::compressWithHeader(sections);
}
