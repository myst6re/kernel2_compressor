#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QString>
#include "kernel2.h"

class Compressor
{
public:
	Compressor();
	explicit Compressor(const QString &fileName);

	inline const QString &fileName() const {
		return _fileName;
	}
	inline void setFileName(const QString &fileName) {
		_fileName = fileName;
	}

	bool open();
	bool save(const QString &fileName, bool optimize,
	          bool doNotBreakFileFormat) const;
	int uncompressedFileSize() const;
	int uncompressedFileSizeAfterCleaning() const;
	int uncompressedFileSizeAfterOptimization() const;
	int uncompressedFileSizeAfterAgressiveOptimization() const;
private:
	QString _fileName;
	Kernel2 _kernel2;
};

#endif // COMPRESSOR_H
