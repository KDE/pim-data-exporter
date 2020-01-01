/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pimdataexporter-version.h"
#include "pimdataexporterconsole.h"
#include "pimdataexportconsole_debug.h"
#include <KAboutData>
#include <KLocalizedString>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QCommandLineOption>

#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    KAboutData aboutData(QStringLiteral("pimdataexporterconsole"), i18n("PIM Data Exporter Console"),
                         QStringLiteral(PIMDATAEXPORTER_VERSION), i18n("PIM Data Exporter Console"), KAboutLicense::GPL_V2,
                         i18n("Copyright © 2015-2020 pimdataexporter authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("logfile"), i18n("File to log information to."), QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("template"), i18n("Template file to define what data, settings to import or export."), QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("import"), i18n("Import the given file."), QStringLiteral("file")));
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("export"), i18n("Export the given file."), QStringLiteral("file")));

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

    PimDataExporterConsole *console = new PimDataExporterConsole;
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
