/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportcalendarsjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "exportcalendarsjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <QTest>
#include <memory>
QTEST_MAIN(ExportCalendarsJobInterfaceTest)

ExportCalendarsJobInterfaceTest::ExportCalendarsJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportCalendarsJobInterfaceTest::exportCalendar_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("calendaronlyconfig") << pathConfig + QByteArray("calendaronlyconfig/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("calendarconfigandresources") << pathConfig + QByteArray("calendarconfigandresources/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("calendarconfigandresourceskalendar") << pathConfig + QByteArray("calendarconfigandresourceskalendar/") << options;
}

void ExportCalendarsJobInterfaceTest::exportCalendar()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);
    QList<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = u"akonadi_icaldir_resource_1"_s;
    lstInfo << info;
    info.identifier = u"akonadi_ical_resource_2"_s;
    lstInfo << info;
    info.identifier = u"akonadi_davgroupware_resource_0"_s;
    lstInfo << info;
    // Add extra resource.
    info.identifier = u"akonadi_contacts_resource_1"_s;
    lstInfo << info;
    info.identifier = u"akonadi_kontact_resource_2"_s;
    lstInfo << info;

    auto exportNote = new ExportCalendarsJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1StringView(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

#include "moc_exportcalendarsjobinterfacetest.cpp"
