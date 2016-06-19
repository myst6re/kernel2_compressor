#include "global.h"
#ifndef KERNEL2_CONSOLE
#	include "mainwindow.h"
#	include <QApplication>
#else
#	include "maincli.h"
#	include <QCoreApplication>
#	include <QTextCodec>
#endif

int main(int argc, char *argv[])
{
#ifndef KERNEL2_CONSOLE
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName(PROG_NAME);
	QCoreApplication::setApplicationVersion(PROG_VERSION);

	MainWindow w;
	w.show();

	return a.exec();
#else
	QCoreApplication a(argc, argv);
	Q_UNUSED(a)
	QCoreApplication::setApplicationName(PROG_NAME);
	QCoreApplication::setApplicationVersion(PROG_VERSION);

#ifdef Q_OS_WIN
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 850"));
#endif // Q_OS_WIN

	MainCLI cli;
	return cli.start();
#endif
}
