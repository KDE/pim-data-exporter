/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "pimdatacommandlineoption.h"
using namespace Qt::Literals::StringLiterals;

#include "config-pimdataexporter.h"
#include "pimdataexporter-version.h"
#include "pimdataexporterwindow.h"
#include "pimdataexportgui_debug.h"
#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineOption>

PimDataCommandLineOption::PimDataCommandLineOption(QObject *parent)
    : QObject(parent)
{
}

PimDataCommandLineOption::~PimDataCommandLineOption() = default;

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
    KAboutData aboutData(u"pimdataexporter"_s,
                         i18n("PIM Data Exporter"),
                         QStringLiteral(PIMDATAEXPORTER_VERSION),
                         i18n("PIM Data Exporter"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2012-%1 pimdataexporter authors", u"2026"_s));
    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), u"montel@kde.org"_s);
    KAboutData::setApplicationData(aboutData);
    aboutData.setupCommandLine(&mParser);
    mParser.addOption(
        QCommandLineOption(QStringList() << u"template"_s, i18n("Template file uses to define what data, settings to import or export"), u"file"_s));
    mParser.addOption(QCommandLineOption(QStringList() << u"import"_s, i18nc("@info:shell", "Import the given file")));
    mParser.addOption(QCommandLineOption(QStringList() << u"export"_s, i18nc("@info:shell", "Export the given file")));
    mParser.addOption(
        QCommandLineOption(QStringList() << u"+[url]"_s, i18nc("@info:shell", "File or url. The user will be asked whether to import or export.")));
#if PIMDATAEXPORTER_WITH_KUSERFEEDBACK
    mParser.addOption(QCommandLineOption(u"feedback"_s, i18nc("@info:shell", "Lists the available options for user feedback")));
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
    return mParser.isSet(u"feedback"_s);
}

#include "moc_pimdatacommandlineoption.cpp"
