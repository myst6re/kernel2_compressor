#ifndef KERNEL2_H
#define KERNEL2_H

#include <QtCore>
#include "core/ff7text.h"

#define KERNEL2_MAX_UNCOMPRESSED_SIZE 27648

class Kernel2
{
public:
	Kernel2();
	bool open(const QString &path);
	bool open(const QByteArray &data);
	bool save(const QString &path, bool sharedData,
	          bool doNotBreakFileFormat) const;
	bool saveUncompressed(QByteArray &data, bool sharedData,
	                      bool doNotBreakFileFormat) const;
	bool save(QByteArray &data, bool sharedData,
	          bool doNotBreakFileFormat) const;
	bool extractAll(const QString &dirPath, const QString &filename) const;
	inline const QList<QByteArray> &texts(int id) const {
		return sections.at(id);
	}
	inline int uncompressedDataSize() const {
		return _uncompressedDataSize;
	}
private:
	bool saveSection(const QList<QByteArray> &texts, QByteArray &data,
	                 bool sharedData, bool doNotBreakFileFormat) const;
	bool extractSection(const QList<QByteArray> &texts,
	                    const QString &path) const;
	QList< QList<QByteArray> > sections;
	int _uncompressedDataSize;
};

#endif // KERNEL2_H
