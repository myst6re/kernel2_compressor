#ifndef KERNEL2GENERATOR_H
#define KERNEL2GENERATOR_H

#include <QByteArray>
#include <QList>
#include <QMap>

struct Kernel2GeneratorTocAction
{
	enum Action {
		Invert,
		Merge
	};

	Kernel2GeneratorTocAction(Action action, int targetIndex, int padding = 0) :
	    action(action), targetIndex(targetIndex), padding(padding) {}

	Action action;
	int targetIndex, padding;
};

class Kernel2Generator
{
public:
	static QByteArray emptyText(int size);
	static QList<QByteArray> emptyTexts(int count, int size);
	static QByteArray randomSafeText(int size);
	static QList<QByteArray> randomSafeTexts(int count, int size);
	static QByteArray section(const QList<QByteArray> &texts,
	                      const QMap<int, Kernel2GeneratorTocAction> &tocActions
	                            = QMap<int, Kernel2GeneratorTocAction>());
	static QByteArray sections(int count, int textCount);
	static QByteArray generate(const QByteArray &sections);
};

#endif // KERNEL2GENERATOR_H
