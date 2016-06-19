#ifndef MAINCLI_H
#define MAINCLI_H

#include <QObject>

class MainCLI : public QObject
{
	Q_OBJECT
public:
	MainCLI();
	int start();
private:
	int exec(const QString &action, const QString &fileName,
	         const QString &outputFileName,
	         const QStringList &options);

};

#endif // MAINCLI_H
