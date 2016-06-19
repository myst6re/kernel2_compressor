#include "mainwindow.h"

#include <QStringList>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include "global.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
	setWindowTitle(PROG_FULLNAME);
	resize(320, 240);

	QPushButton *selectFile = new QPushButton(tr("Open kernel2.bin file..."),
	                                          this);
	_saveAs = new QPushButton(tr("Optimize and save as..."),
	                          this);
	_saveAs->setEnabled(false);
	_display = new QLabel(this);
	_display->setTextFormat(Qt::PlainText);
	_display->setWordWrap(true);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(selectFile, 0, 0);
	layout->addWidget(_saveAs, 0, 1);
	layout->addWidget(_display, 1, 0, 1, 2);
	layout->setRowStretch(2, 1);

	connect(selectFile, SIGNAL(clicked()), SLOT(selectFile()));
	connect(_saveAs, SIGNAL(clicked()), SLOT(saveAs()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::selectFile()
{
	QStringList filter;
	QString fileName;

	filter << tr("BIN files (*.bin)") << tr("All files (*)");

	fileName = QFileDialog::getOpenFileName(this, tr("Open kernel2.bin file"),
	                             QString(), // TODO: directory
	                             filter.join(";;")); // TODO: selectedFilter

	if (fileName.isNull()) {
		return;
	}

	openFile(fileName);
}

void MainWindow::saveAs()
{
	QStringList filter;
	QString fileName;

	filter << tr("BIN files (*.bin)") << tr("All files (*)");

	fileName = QFileDialog::getSaveFileName(this, tr("Open kernel2.bin file"),
	                             QString(), // TODO: directory
	                             filter.join(";;")); // TODO: selectedFilter

	if (fileName.isNull()) {
		return;
	}

	saveFileOptimized(fileName);
}

void MainWindow::openFile(const QString &fileName)
{
	_compressor.setFileName(fileName);
	if (!_compressor.open()) {
		// TODO: error message
		return;
	}
	_saveAs->setEnabled(true);
	showFileInfos();
}

void MainWindow::saveFileOptimized(const QString &fileName)
{
	if (!_compressor.saveOptimized(fileName)) {
		// TODO: error message
		return;
	}
}

void MainWindow::showFileInfos()
{
	_display->setText(tr("Uncompressed size: %1 B\n"
	                     "Max authorized size: %2 B\n"
	                     "Cleaned size: %3 B\n"
	                     "Optimized size: %4 B")
	                  .arg(_compressor.uncompressedFileSize())
	                  .arg(KERNEL2_MAX_UNCOMPRESSED_SIZE)
	                  .arg(_compressor.uncompressedFileSizeAfterCleaning())
	                  .arg(_compressor.uncompressedFileSizeAfterOptimization())
	                  );
}


