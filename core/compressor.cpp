#include "compressor.h"

Compressor::Compressor()
{
}

Compressor::Compressor(const QString &fileName) : _fileName(fileName)
{
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

QString Compressor::infos() const
{
	return QObject::tr("Uncompressed size: %1 B\n"
	                   "Max authorized size: %2 B\n"
	                   "Cleaned size: %3 B\n"
	                   "Optimized size: %4 B\n"
	                   "Optimized best size: %5 B")
	        .arg(uncompressedFileSize())
	        .arg(KERNEL2_MAX_UNCOMPRESSED_SIZE)
	        .arg(uncompressedFileSizeAfterCleaning())
	        .arg(uncompressedFileSizeAfterOptimization())
	        .arg(uncompressedFileSizeAfterAgressiveOptimization());
}
