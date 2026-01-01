/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexportconsole_debug.h"
using namespace Qt::Literals::StringLiterals;

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
    KAboutData aboutData(u"pimdataexporterconsole"_s,
                         i18n("PIM Data Exporter Console"),
                         QStringLiteral(PIMDATAEXPORTER_VERSION),
                         i18n("PIM Data Exporter Console"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2015-%1 pimdataexporter authors", u"2026"_s));
    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), u"montel@kde.org"_s);
    const QCommandLineOption logOption(QStringList() << u"logfile"_s, i18nc("@info:shell", "File to log information to."), u"file"_s);
    parser.addOption(logOption);
    const QCommandLineOption templateOption(QStringList() << u"template"_s,
                                            i18nc("@info:shell", "Template file to define what data, settings to import or export."),
                                            u"file"_s);
    parser.addOption(templateOption);
    const QCommandLineOption importOption(QStringList() << u"import"_s, i18nc("@info:shell", "Import the given file."), u"file"_s);
    parser.addOption(importOption);
    const QCommandLineOption exportOption(QStringList() << u"export"_s, i18nc("@info:shell", "Export the given file."), u"file"_s);
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
