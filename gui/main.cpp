/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatacommandlineoption.h"
#include "pimdataexporterwindow.h"

#include <KCrash>
#if HAVE_KDBUSADDONS
#include <KDBusService>
#endif

#include <KLocalizedString>
#include <QApplication>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedback/Provider>
#endif

#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif

int main(int argc, char *argv[])
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("pimdataexporter"));

#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif
#endif
    app.setDesktopFileName(QStringLiteral("org.kde.pimdataexporter"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kontact")));

    PimDataCommandLineOption parser;
    parser.createParser(app);
    KCrash::initialize();
#ifdef WITH_KUSERFEEDBACK
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
    QObject::connect(&service, &KDBusService::activateRequested, &parser, &PimDataCommandLineOption::slotActivateRequested);
    backupMailWin->show();
    parser.handleCommandLine();

    return app.exec();
}
