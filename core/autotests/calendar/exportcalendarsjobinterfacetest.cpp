/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "exportcalendarsjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testexportfile.h"
#include <QDebug>
#include <QTest>
#include <saveresourceconfigtest.h>
#include <testbackupresourcefilejob.h>

QTEST_MAIN(ExportCalendarsJobInterfaceTest)

ExportCalendarsJobInterfaceTestImpl::ExportCalendarsJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportCalendarsJobInterfaceTestImpl::~ExportCalendarsJobInterfaceTestImpl()
{
}

Akonadi::Collection::Id ExportCalendarsJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ExportCalendarsJobInterfaceTestImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertToFullCollectionPath(collectionValue);
}

void ExportCalendarsJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResoureConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceCalendarArchiveName(), { QLatin1String("akonadi_icaldir_resource_")});
    slotCalendarJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportCalendarsJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportCalendarsJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    converter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportCalendarsJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportCalendarsJobInterfaceTestImpl::backupCalendarResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    TestBackupResourceFileJob *job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setTestPath(mPathConfig);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportCalendarsJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportCalendarsJobInterfaceTestImpl::info);
    job->start();
}

QString ExportCalendarsJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

ExportCalendarsJobInterfaceTest::ExportCalendarsJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("calendaronlyconfig") << pathConfig + QByteArray("calendaronlyconfig/");
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportCalendarsJobInterfaceTestImpl *impl = new ExportCalendarsJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    impl->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfigAndResource_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("calendarconfigandconfig") << pathConfig + QByteArray("calendarconfigandconfig/");
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfigAndResource()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = QLatin1String("akonadi_icaldir_resource_1");
    lstInfo << info;
    info.identifier = QLatin1String("akonadi_ical_resource_2");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QStringLiteral("akonadi_kontact_resource_2");
    lstInfo << info;

    ExportCalendarsJobInterfaceTestImpl *exportNote = new ExportCalendarsJobInterfaceTestImpl(this, {Utils::StoredType::Config|Utils::StoredType::Resources}, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
