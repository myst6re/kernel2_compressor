#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "core/compressor.h"

class QLabel;
class QPushButton;
class QComboBox;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();
public slots:
	void selectFile();
	void saveAs();

private:
	void openFile(const QString &fileName);
	void saveFileOptimized(const QString &fileName);
	void showFileInfos();
	void showError();

	Compressor _compressor;

	QPushButton *_saveAs;
	QLabel *_display;
	QComboBox *_saveOptions;
};

#endif // MAINWINDOW_H
