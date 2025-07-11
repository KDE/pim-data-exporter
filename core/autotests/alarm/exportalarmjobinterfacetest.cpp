/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportalarmjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "exportalarmjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <QTest>

QTEST_MAIN(ExportAlarmJobInterfaceTest)

ExportAlarmJobInterfaceTest::ExportAlarmJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAlarmJobInterfaceTest::exportAlarm_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("alarmonlyconfig") << pathConfig + QByteArray("alarmonlyconfig/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("alarmconfigresources") << pathConfig + QByteArray("alarmconfigresources/") << options;
}

void ExportAlarmJobInterfaceTest::exportAlarm()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);
    QList<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = u"akonadi_kalarm_dir_resource_1"_s;
    lstInfo << info;
    info.identifier = u"akonadi_kalarm_dir_resource_2"_s;
    lstInfo << info;
    // Add extra resource.
    info.identifier = u"akonadi_kolab_resource_2"_s;
    lstInfo << info;

    auto exportNote = new ExportAlarmJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1StringView(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

#include "moc_exportalarmjobinterfacetest.cpp"
