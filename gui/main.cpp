/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatacommandlineoption.h"
#include "pimdataexporterwindow.h"

#include <KDBusService>
#include <KLocalizedString>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <Kdelibs4ConfigMigrator>
#endif
#include <KCrash>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/pimdataexporteduserfeedbackprovider.h"
#ifdef USE_KUSERFEEDBACK_QT6
#include <KUserFeedbackQt6/Provider>
#else
#include <KUserFeedback/Provider>
#endif
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("pimdataexporter");

    KCrash::initialize();
    app.setDesktopFileName(QStringLiteral("org.kde.pimdataexporter"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Kdelibs4ConfigMigrator migrate(QStringLiteral("pimdataexporter"));
    // old migration. Don't change it
    migrate.setConfigFiles(QStringList() << QStringLiteral("pimsettingexporterrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("pimsettingexporter.rc"));
    migrate.migrate();
#endif
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
