/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_UNUSED(workingDirectory)
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
                         i18n("Copyright © 2012-2020 pimdataexporter authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));
    KAboutData::setApplicationData(aboutData);
    aboutData.setupCommandLine(&mParser);
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("template"), i18n("Template file uses to define what data, settings to import or export"), QStringLiteral("file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("import"), i18n("Import the given file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("export"), i18n("Export the given file")));
    mParser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("+[url]"), i18n("File or url. The user will be asked whether to import or export.")));
#ifdef WITH_KUSERFEEDBACK
    mParser.addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif
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

bool PimDataCommandLineOption::parseUserFeedback() const
{
    return mParser.isSet(QStringLiteral("feedback"));
}
