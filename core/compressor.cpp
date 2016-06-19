#include "compressor.h"

Compressor::Compressor()
{
}

Compressor::Compressor(const QString &fileName) :
    _fileName(fileName)
{
}

bool Compressor::open()
{
	return _kernel2.open(_fileName);
}

bool Compressor::saveOptimized(const QString &fileName) const
{
	return _kernel2.save(fileName, true);
}

int Compressor::uncompressedFileSize() const
{
	return _kernel2.uncompressedDataSize();
}

int Compressor::uncompressedFileSizeAfterCleaning() const
{
	QByteArray data;
	if (_kernel2.saveUncompressed(data, false)) {
		return data.size();
	}

	return -1;
}

int Compressor::uncompressedFileSizeAfterOptimization() const
{
	QByteArray data;
	if (_kernel2.saveUncompressed(data, true)) {
		return data.size();
	}

	return -1;
}
