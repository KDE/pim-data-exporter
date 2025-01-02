/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "config-pimdataexporter.h"
#include "pimdatacommandlineoption.h"
#include "pimdataexporterwindow.h"

#include <KCrash>
#if HAVE_KDBUSADDONS
#include <KDBusService>
#endif

#include <KLocalizedString>
#include <QApplication>
#if PIMDATAEXPORTER_WITH_KUSERFEEDBACK
#include "userfeedback/pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedback/Provider>
#endif

#include <KIconTheme>

#include <KStyleManager>

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("pimdataexporter"));

    KStyleManager::initStyle();
    app.setDesktopFileName(QStringLiteral("org.kde.pimdataexporter"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kontact")));

    PimDataCommandLineOption parser;
    parser.createParser(app);
    KCrash::initialize();
#if PIMDATAEXPORTER_WITH_KUSERFEEDBACK
    if (parser.parseUserFeedback()) {
        auto provider = new PimDataExportedUserFeedbackProvider;
        QTextStream(stdout) << provider->describeDataSources() << '\n';
        delete provider;
        return 0;
    }
#endif

#if HAVE_KDBUSADDONS
    KDBusService service(KDBusService::Unique);
#endif

    auto backupMailWin = new PimDataExporterWindow();
    parser.setExportWindow(backupMailWin);
#if HAVE_KDBUSADDONS
    QObject::connect(&service, &KDBusService::activateRequested, &parser, &PimDataCommandLineOption::slotActivateRequested);
#endif
    backupMailWin->show();
    parser.handleCommandLine();

    return app.exec();
}
