#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QString>
#include "kernel2.h"

class Compressor
{
public:
	Compressor();
	explicit Compressor(const QString &fileName);

	inline const QString &fileName() const
	{
		return _fileName;
	}
	inline void setFileName(const QString &fileName)
	{
		_fileName = fileName;
	}

	inline bool open() {
		return _kernel2.open(_fileName);
	}
	inline bool save(const QString &fileName, bool optimize,
	          bool doNotBreakFileFormat) const {
		return _kernel2.save(fileName, optimize, doNotBreakFileFormat);
	}
	inline QString errorString() const {
		return _kernel2.errorString();
	}
	inline int uncompressedFileSize() const {
		return _kernel2.uncompressedDataSize();
	}
	int uncompressedFileSizeAfterCleaning() const;
	int uncompressedFileSizeAfterOptimization() const;
	int uncompressedFileSizeAfterAgressiveOptimization() const;
	QString infos() const;

private:
	QString _fileName;
	Kernel2 _kernel2;
};

#endif // COMPRESSOR_H
