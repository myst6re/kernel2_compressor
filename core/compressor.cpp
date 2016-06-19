#include "compressor.h"

Compressor::Compressor()
{
}

Compressor::Compressor(const QString &fileName) : _fileName(fileName)
{
}

bool Compressor::open()
{
	return _kernel2.open(_fileName);
}

bool Compressor::save(const QString &fileName, bool optimize,
                      bool doNotBreakFileFormat) const
{
	return _kernel2.save(fileName, optimize, doNotBreakFileFormat);
}

int Compressor::uncompressedFileSize() const
{
	return _kernel2.uncompressedDataSize();
}

int Compressor::uncompressedFileSizeAfterCleaning() const
{
	QByteArray data;
	if (_kernel2.saveUncompressed(data, false, true)) {
		return data.size();
	}

	return -1;
}

int Compressor::uncompressedFileSizeAfterOptimization() const
{
	QByteArray data;
	if (_kernel2.saveUncompressed(data, true, true)) {
		return data.size();
	}

	return -1;
}

int Compressor::uncompressedFileSizeAfterAgressiveOptimization() const
{
	QByteArray data;
	if (_kernel2.saveUncompressed(data, true, true)) {
		return data.size();
	}

	return -1;
}
