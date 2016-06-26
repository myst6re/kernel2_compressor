#include "kernel2test.h"
#include "core/kernel2.h"
#include "tests/kernel2generator.h"

void Kernel2Test::itShouldOpenTexts()
{
	QList<QByteArray> texts1 = Kernel2Generator::randomSafeTexts(3, 4),
			texts2 = Kernel2Generator::randomSafeTexts(3, 6);
	QByteArray sections = Kernel2Generator::section(texts1)
	                      .append(Kernel2Generator::section(texts2)),
	        kernel2Data = Kernel2Generator::generate(sections);

	Kernel2 kernel2;
	if (kernel2.open(kernel2Data)) {
		QCOMPARE(kernel2.sections().size(), 2);
		QCOMPARE(kernel2.sections().at(0), texts1);
		QCOMPARE(kernel2.sections().at(1), texts2);
	} else {
		QFAIL(qPrintable(kernel2.errorString()));
	}
}

void Kernel2Test::itShouldOpenInvertedTexts()
{
	QFAIL("Not implemented test"); // TODO
}

void Kernel2Test::itShouldOpenMergedTexts()
{
	QFAIL("Not implemented test"); // TODO
}

void Kernel2Test::itShouldSaveWithoutChange()
{
	QList<QByteArray> texts1 = Kernel2Generator::randomSafeTexts(3, 16),
			texts2 = Kernel2Generator::randomSafeTexts(3, 32);
	QByteArray kernel2Data = Kernel2Generator::generate(
	                             Kernel2Generator::section(texts1)
	                             .append(Kernel2Generator::section(texts2)));
	Kernel2 kernel2;
	if (kernel2.open(kernel2Data)) {
		QByteArray newData;
		if (kernel2.save(newData, false, true)) {
			QCOMPARE(newData, kernel2Data);
		} else {
			QFAIL("Cannot save");
		}
	} else {
		QFAIL(qPrintable(kernel2.errorString()));
	}
}

void Kernel2Test::itShouldSaveWithCleanedFf()
{
	QList<QByteArray> texts = Kernel2Generator::emptyTexts(2, 0);
	QByteArray kernel2Data = Kernel2Generator::generate(
	                             Kernel2Generator::section(texts));
	Kernel2 kernel2, kernel2New;
	if (kernel2.open(kernel2Data)) {
		QByteArray newData;
		if (kernel2.save(newData, false, true)) {
			if (kernel2New.open(newData)) {
				QCOMPARE(kernel2.sections(), kernel2New.sections());
				QVERIFY(kernel2Data != newData);
			} else {
				QFAIL(qPrintable(kernel2.errorString()));
			}
		} else {
			QFAIL("Cannot save");
		}
	} else {
		QFAIL(qPrintable(kernel2.errorString()));
	}
}

void Kernel2Test::itShouldSaveWithInvertedTexts()
{
	QFAIL("Not implemented test"); // TODO
}

void Kernel2Test::itShouldSaveWithMergedTexts()
{
	QList<QByteArray> texts = Kernel2Generator::emptyTexts(3, 6);
	QByteArray kernel2Data = Kernel2Generator::generate(
	                             Kernel2Generator::section(texts));
	Kernel2 kernel2, kernel2New;
	if (kernel2.open(kernel2Data)) {
		QByteArray newData;
		if (kernel2.save(newData, true, true)) {
			if (kernel2New.open(newData)) {
				QCOMPARE(kernel2.sections(), kernel2New.sections());
				QVERIFY(kernel2Data != newData);
			} else {
				QFAIL(qPrintable(kernel2.errorString()));
			}
		} else {
			QFAIL("Cannot save");
		}
	} else {
		QFAIL(qPrintable(kernel2.errorString()));
	}
}

