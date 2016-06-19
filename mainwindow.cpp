#include "mainwindow.h"

#include <QStringList>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include "global.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	setWindowTitle(PROG_FULLNAME);
	resize(320, 240);

	QPushButton *selectFile =
	    new QPushButton(tr("Open kernel2.bin file..."), this);

	_saveAs = new QPushButton(tr("Save kernel2.bin file as..."), this);
	_saveAs->setEnabled(false);

	_display = new QLabel(this);
	_display->setTextFormat(Qt::PlainText);
	_display->setWordWrap(true);

	_saveOptions = new QComboBox(this);
	_saveOptions->addItem(tr("Do not optimize (clean only)"));
	_saveOptions->addItem(tr("Optimize"));
	_saveOptions->addItem(tr("Optimize best"));
	_saveOptions->setEnabled(false);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(selectFile, 0, 0);
	layout->addWidget(_display, 1, 0);
	layout->addWidget(_saveOptions, 2, 0);
	layout->addWidget(_saveAs, 3, 0);
	layout->setRowStretch(1, 1);

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

	fileName =
	    QFileDialog::getOpenFileName(this, tr("Open kernel2.bin file"),
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

	fileName =
	    QFileDialog::getSaveFileName(this, tr("Open kernel2.bin file"),
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
	_saveOptions->setEnabled(true);
	showFileInfos();
}

void MainWindow::saveFileOptimized(const QString &fileName)
{
	bool optimized = false, doNotBreakFileFormat = true;

	switch (_saveOptions->currentIndex()) {
	case 0:
		optimized = false;
		doNotBreakFileFormat = true;
		break;
	case 1:
		optimized = true;
		doNotBreakFileFormat = true;
		break;
	case 2:
		optimized = true;
		doNotBreakFileFormat = false;
		break;
	default:
		Q_ASSERT(false);
		return;
	}

	if (!_compressor.save(fileName, optimized, doNotBreakFileFormat)) {
		// TODO: error message
		return;
	}
}

void MainWindow::showFileInfos()
{
	_display->setText(_compressor.infos());
}
