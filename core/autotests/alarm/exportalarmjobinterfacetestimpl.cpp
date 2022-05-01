/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportalarmjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include "testbackupresourcefilejob.h"

ExportAlarmJobInterfaceTestImpl::ExportAlarmJobInterfaceTestImpl(QObject *parent,
                                                                 Utils::StoredTypes typeSelected,
                                                                 ArchiveStorage *archiveStorage,
                                                                 int numberOfStep)
    : ExportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAlarmJobInterfaceTestImpl::~ExportAlarmJobInterfaceTestImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportAlarmJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

Akonadi::Collection::Id ExportAlarmJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAlarmJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResourceConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath,
                                               url,
                                               identifier,
                                               Utils::resourceAlarmArchiveName(),
                                               {QLatin1String("akonadi_kalarm_dir_resource_")});
    slotAlarmJobTerminated();
}

QString ExportAlarmJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportAlarmJobInterfaceTestImpl::backupAlarmResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    auto job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setTestPath(mPathConfig);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportAlarmJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportAlarmJobInterfaceTestImpl::info);
    job->start();
}

QString ExportAlarmJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ExportAlarmJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    resourceConverterTest.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}

QString
ExportAlarmJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}
