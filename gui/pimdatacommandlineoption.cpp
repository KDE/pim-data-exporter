/*
   Copyright (C) 2015-2019 Montel Laurent <montel@kde.org>

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
#include "pimdatacommandlineoption.h"
#include "pimdataexporterwindow.h"
#include "pimdataexportgui_debug.h"
#include "pimdataexporter-version.h"
#include <KLocalizedString>
#include <KAboutData>
#include <QCommandLineOption>
#include <QApplication>

PimDataCommandLineOption::PimDataCommandLineOption(QObject *parent)
    : QObject(parent)
{
}

PimDataCommandLineOption::~PimDataCommandLineOption()
{
}

void PimDataCommandLineOption::slotActivateRequested(const QStringList &arguments, const QString &workingDirectory)
{
    Q_UNUSED(workingDirectory);
    if (!arguments.isEmpty()) {
        if (mParser.parse(arguments)) {
            handleCommandLine();
        } else {
            qCDebug(PIMDATAEXPORTERGUI_LOG) << " Impossible to parse argument ";
        }
    }
}

void PimDataCommandLineOption::createParser(const QApplication &app)
{
    KAboutData aboutData(QStringLiteral("pimdataexporter"), i18n("PIM Data Exporter"),
                         QStringLiteral(PIMDATAEXPORTER_VERSION), i18n("PIM Data Exporter"), KAboutLicense::GPL_V2,
                         i18n("Copyright © 2012-2019 pimdataexporter authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    mParser.addVersionOption();
    mParser.addHelpOption();
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("template"), i18n("Template file uses to define what data, settings to import or export"), QStringLiteral("file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("import"), i18n("Import the given file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("export"), i18n("Export the given file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("+[url]"), i18n("File or url. The user will be asked whether to import or export.")));

    KAboutData::setApplicationData(aboutData);

    aboutData.setupCommandLine(&mParser);
    mParser.process(app);
    aboutData.processCommandLine(&mParser);
}

void PimDataCommandLineOption::setExportWindow(PimDataExporterWindow *exporterWindow)
{
    mExporterWindow = exporterWindow;
}

void PimDataCommandLineOption::handleCommandLine()
{
    if (mExporterWindow) {
        mExporterWindow->handleCommandLine(mParser);
    }
}