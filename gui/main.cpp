/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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
        PimDataExportedUserFeedbackProvider *provider = new PimDataExportedUserFeedbackProvider;
        QTextStream(stdout) << provider->describeDataSources() << '\n';
        delete provider;
        return 0;
    }
#endif
    KDBusService service(KDBusService::Unique);

    PimDataExporterWindow *backupMailWin = new PimDataExporterWindow();
    parser.setExportWindow(backupMailWin);
    QObject::connect(&service, &KDBusService::activateRequested,
                     &parser, &PimDataCommandLineOption::slotActivateRequested);
    backupMailWin->show();
    parser.handleCommandLine();

    return app.exec();
}
