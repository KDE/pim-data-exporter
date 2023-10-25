/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatacommandlineoption.h"
#include "pimdataexporterwindow.h"

#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>
#include <QApplication>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedbackQt6/Provider>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("pimdataexporter"));

    KCrash::initialize();
    app.setDesktopFileName(QStringLiteral("org.kde.pimdataexporter"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kontact")));

    PimDataCommandLineOption parser;
    parser.createParser(app);
#ifdef WITH_KUSERFEEDBACK
    if (parser.parseUserFeedback()) {
        auto provider = new PimDataExportedUserFeedbackProvider;
        QTextStream(stdout) << provider->describeDataSources() << '\n';
        delete provider;
        return 0;
    }
#endif
    KDBusService service(KDBusService::Unique);

    auto backupMailWin = new PimDataExporterWindow();
    parser.setExportWindow(backupMailWin);
    QObject::connect(&service, &KDBusService::activateRequested, &parser, &PimDataCommandLineOption::slotActivateRequested);
    backupMailWin->show();
    parser.handleCommandLine();

    return app.exec();
}
