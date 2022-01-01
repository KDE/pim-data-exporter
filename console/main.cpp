/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

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
                         i18n("Copyright © 2015-%1 pimdataexporter authors", QStringLiteral("2022")));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("logfile"), i18n("File to log information to."), QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("template"),
                                        i18n("Template file to define what data, settings to import or export."),
                                        QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("import"), i18n("Import the given file."), QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("export"), i18n("Export the given file."), QStringLiteral("file")));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    QString importFile;
    QString exportFile;
    if (parser.isSet(QStringLiteral("import"))) {
        importFile = parser.value(QStringLiteral("import"));
    } else if (parser.isSet(QStringLiteral("export"))) {
        exportFile = parser.value(QStringLiteral("export"));
    }
    if (importFile.isEmpty() && exportFile.isEmpty()) {
        parser.showHelp();
        return 0;
    }
    QString logFile;
    QString templateFile;
    if (parser.isSet(QStringLiteral("template"))) {
        templateFile = parser.value(QStringLiteral("template"));
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Template file " << templateFile;
    }
    if (parser.isSet(QStringLiteral("logfile"))) {
        logFile = parser.value(QStringLiteral("logfile"));
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
