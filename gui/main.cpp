/*
   SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterwindow.h"
#include "pimdatacommandlineoption.h"

#include <KLocalizedString>
#include <KDBusService>
#include <QApplication>
#include <Kdelibs4ConfigMigrator>
#include <KCrash>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedback/Provider>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("pimdataexporter");

    KCrash::initialize();
    app.setDesktopFileName(QStringLiteral("org.kde.pimdataexporter"));
    Kdelibs4ConfigMigrator migrate(QStringLiteral("pimdataexporter"));
    //old migration. Don't change it
    migrate.setConfigFiles(QStringList() << QStringLiteral("pimsettingexporterrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("pimsettingexporter.rc"));
    migrate.migrate();
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kontact")));

    PimDataCommandLineOption parser;
    parser.createParser(app);
#ifdef WITH_KUSERFEEDBACK
    if (parser.parseUserFeedback()) {
        auto *provider = new PimDataExportedUserFeedbackProvider;
        QTextStream(stdout) << provider->describeDataSources() << '\n';
        delete provider;
        return 0;
    }
#endif
    KDBusService service(KDBusService::Unique);

    auto *backupMailWin = new PimDataExporterWindow();
    parser.setExportWindow(backupMailWin);
    QObject::connect(&service, &KDBusService::activateRequested,
                     &parser, &PimDataCommandLineOption::slotActivateRequested);
    backupMailWin->show();
    parser.handleCommandLine();

    return app.exec();
}
