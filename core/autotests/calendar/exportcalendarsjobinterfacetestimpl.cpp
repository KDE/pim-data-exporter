/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportcalendarsjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include "testbackupresourcefilejob.h"
#include <QDebug>

ExportCalendarsJobInterfaceTestImpl::ExportCalendarsJobInterfaceTestImpl(QObject *parent,
                                                                         Utils::StoredTypes typeSelected,
                                                                         ArchiveStorage *archiveStorage,
                                                                         int numberOfStep)
    : ExportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportCalendarsJobInterfaceTestImpl::~ExportCalendarsJobInterfaceTestImpl() = default;

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
    qDebug() << " void ExportCalendarsJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)"
             << identifier;
    SaveResourceConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath,
                                               url,
                                               identifier,
                                               Utils::resourceCalendarArchiveName(),
                                               {QLatin1StringView("akonadi_icaldir_resource_")});
    slotCalendarJobTerminated();
}

QList<Utils::AkonadiInstanceInfo> ExportCalendarsJobInterfaceTestImpl::listOfResource()
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
    auto job = new TestBackupResourceFileJob(this);
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

QString ExportCalendarsJobInterfaceTestImpl::createResource(const QString &resources,
                                                            const QString &name,
                                                            const QMap<QString, QVariant> &settings,
                                                            bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(settings);
    Q_UNUSED(name);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}

#include "moc_exportcalendarsjobinterfacetestimpl.cpp"
