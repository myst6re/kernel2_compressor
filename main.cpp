#ifndef NM_CONSOLE
#	include "mainwindow.h"
#	include <QApplication>
#else
#	include <QCoreApplication>
#endif

int main(int argc, char *argv[])
{
#ifndef NM_CONSOLE
	QApplication a(argc, argv);

	MainWindow w;
	w.show();
#else
	QCoreApplication a(argc, argv);

#ifdef Q_OS_WIN
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 850"));
#endif // Q_OS_WIN

#endif

	return a.exec();
}
