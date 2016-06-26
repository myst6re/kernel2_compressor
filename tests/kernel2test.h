#ifndef KERNEL2TEST_H
#define KERNEL2TEST_H

#include <QtTest/QtTest>

class Kernel2Test : public QObject
{
	Q_OBJECT
public:
	Kernel2Test() {}

private slots:
	void itShouldOpenTexts();
	void itShouldOpenInvertedTexts();
	void itShouldOpenMergedTexts();
	void itShouldSaveWithoutChange();
	void itShouldSaveWithCleanedFf();
	void itShouldSaveWithInvertedTexts();
	void itShouldSaveWithMergedTexts();
};

#endif // KERNEL2TEST_H
