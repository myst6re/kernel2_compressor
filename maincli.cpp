#include "maincli.h"

#include <QCommandLineParser>
#include <QCoreApplication>

#include "core/compressor.h"

MainCLI::MainCLI()
{
}

int MainCLI::start()
{
	QStringList args = QCoreApplication::arguments();

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("action",
	                             tr("Action to do: clean, help, infos "
	                                "or optimize"));
	parser.parse(args);

	if (parser.isSet(QStringLiteral("version"))) {
		parser.showVersion();
	}

	if (parser.isSet(QStringLiteral("help"))) {
		parser.showHelp(EXIT_SUCCESS);
	}

	QStringList posArgs = parser.positionalArguments();

	if (posArgs.isEmpty()) {
		qCritical(qPrintable(tr("Please specify the action.")));
		return EXIT_FAILURE;
	}
	
	QString action = posArgs.first();

	parser.clearPositionalArguments();
	
	bool showCommandHelp = "help" == action;

	if (showCommandHelp) {
		parser.addPositionalArgument("command",
									 tr("Show help of specific command"));

		parser.parse(args);

		posArgs = parser.positionalArguments();

		if (posArgs.size() < 2) {
			qCritical(qPrintable(tr("Please specify the action.")));
			return EXIT_FAILURE;
		}

		action = posArgs.at(1);
	}

	parser.clearPositionalArguments();

	if ("clean" == action) {
		parser.addPositionalArgument(action,
		                             tr("Clean kernel2.bin file and "
		                                "remove optimizations."));
		parser.addPositionalArgument("filename",
		                             tr("Source kernel2.bin file name"));
		parser.addPositionalArgument("output",
		                             tr("Target kernel2.bin file name"));
	} else if ("infos" == action) {
		parser.addPositionalArgument(action,
		                             tr("Show infos about kernel2.bin sizes."));
		parser.addPositionalArgument("filename",
		                             tr("Source kernel2.bin file name"));
	} else if ("optimize" == action) {
		parser.addPositionalArgument(action,
		                             tr("Compress kernel2.bin file."));
		parser.addPositionalArgument("filename",
		                             tr("Source kernel2.bin file name"));
		parser.addPositionalArgument("output",
		                             tr("Target kernel2.bin file name"));
		parser.addOption(QCommandLineOption(
		                     "best", tr("Compress more, "
		                                "can break the file format for "
		                                "kernel2.bin editors")));
	} else {
		qCritical(qPrintable(tr("Unknown action. "
		                        "Available actions: clean, infos, optimize")));
		return EXIT_FAILURE;
	}

	if (showCommandHelp) {
		parser.showHelp(EXIT_SUCCESS);
	}

	parser.process(args);

	posArgs = parser.positionalArguments();

	if (posArgs.size() < 2) {
		qCritical(qPrintable(tr("Please specify the source file name.")));
		return EXIT_FAILURE;
	}

	return exec(action, posArgs.at(1),
	            posArgs.value(2),
	            parser.optionNames());
}

int MainCLI::exec(const QString &action, const QString &fileName,
                  const QString &outputFileName,
                  const QStringList &options)
{
	Compressor compressor(fileName);

	if (!compressor.open()) {
		qCritical(qPrintable(tr("Cannot open input file '%1'")
		                     .arg(fileName)));
		return EXIT_FAILURE;
	}

	bool optimize = action == "optimize";

	if (action == "clean" || optimize) {
		if (outputFileName.isEmpty()) {
			qCritical(qPrintable(tr("Please specify the output file name.")));
			return EXIT_FAILURE;
		}
		bool breakFileFormat = optimize && options.contains("best");
		if (!compressor.save(outputFileName, optimize, !breakFileFormat)) {
			qCritical(qPrintable(tr("Cannot save to output file '%1'")
			                     .arg(outputFileName))); // FIXME: error message
			return EXIT_FAILURE;
		}
		qInfo(qPrintable(compressor.infos()));
	} else if (action == "infos") {
		qInfo(qPrintable(compressor.infos()));
	}

	return EXIT_SUCCESS;
}
