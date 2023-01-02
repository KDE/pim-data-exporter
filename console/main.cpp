/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexportconsole_debug.h"
#include "pimdataexporter-version.h"
#include "pimdataexporterconsole.h"
#include <KAboutData>
#include <KLocalizedString>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    KAboutData aboutData(QStringLiteral("pimdataexporterconsole"),
                         i18n("PIM Data Exporter Console"),
                         QStringLiteral(PIMDATAEXPORTER_VERSION),
                         i18n("PIM Data Exporter Console"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2015-%1 pimdataexporter authors", QStringLiteral("2023")));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    const QCommandLineOption logOption(QStringList() << QStringLiteral("logfile"), i18n("File to log information to."), QStringLiteral("file"));
    parser.addOption(logOption);
    const QCommandLineOption templateOption(QStringList() << QStringLiteral("template"),
                                            i18n("Template file to define what data, settings to import or export."),
                                            QStringLiteral("file"));
    parser.addOption(templateOption);
    const QCommandLineOption importOption(QStringList() << QStringLiteral("import"), i18n("Import the given file."), QStringLiteral("file"));
    parser.addOption(importOption);
    const QCommandLineOption exportOption(QStringList() << QStringLiteral("export"), i18n("Export the given file."), QStringLiteral("file"));
    parser.addOption(exportOption);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    QString importFile;
    QString exportFile;
    if (parser.isSet(importOption)) {
        importFile = parser.value(importOption);
    } else if (parser.isSet(exportOption)) {
        exportFile = parser.value(exportOption);
    }
    if (importFile.isEmpty() && exportFile.isEmpty()) {
        parser.showHelp();
        return 0;
    }
    QString logFile;
    QString templateFile;
    if (parser.isSet(templateOption)) {
        templateFile = parser.value(templateOption);
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Template file " << templateFile;
    }
    if (parser.isSet(logOption)) {
        logFile = parser.value(logOption);
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Log file " << logFile;
    }

    auto console = new PimDataExporterConsole;
    if (!importFile.isEmpty()) {
        console->setMode(PimDataExporterConsole::Import);
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Import Mode" << importFile;
        console->setImportExportFileName(importFile);
    } else if (!exportFile.isEmpty()) {
        console->setMode(PimDataExporterConsole::Export);
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Export Mode" << exportFile;
        console->setImportExportFileName(exportFile);
    }
    if (!logFile.isEmpty()) {
        console->setLogFileName(logFile);
    }
    if (!templateFile.isEmpty()) {
        console->setTemplateFileName(templateFile);
    }
    QObject::connect(console, &PimDataExporterConsole::finished, &app, &QCoreApplication::quit);
    QTimer::singleShot(0, console, &PimDataExporterConsole::start);

    return app.exec();
}
