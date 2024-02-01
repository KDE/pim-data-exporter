/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "exportaddressbookjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include "testbackupresourcefilejob.h"

ExportAddressbookJobInterfaceTestImpl::ExportAddressbookJobInterfaceTestImpl(QObject *parent,
                                                                             Utils::StoredTypes typeSelected,
                                                                             ArchiveStorage *archiveStorage,
                                                                             int numberOfStep)
    : ExportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAddressbookJobInterfaceTestImpl::~ExportAddressbookJobInterfaceTestImpl() = default;

Akonadi::Collection::Id ExportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAddressbookJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResourceConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath,
                                               url,
                                               identifier,
                                               Utils::resourceAddressbookArchiveName(),
                                               {QLatin1StringView("akonadi_vcarddir_resource_"), QLatin1StringView("akonadi_contacts_resource_")});
    slotAddressbookJobTerminated();
}

QList<Utils::AkonadiInstanceInfo> ExportAddressbookJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportAddressbookJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ExportAddressbookJobInterfaceTestImpl::createResource(const QString &resources,
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

QString ExportAddressbookJobInterfaceTestImpl::resourcePath(const QString &agentIdentifier, const QString &defaultPath) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(agentIdentifier, defaultPath);
    return url;
}

void ExportAddressbookJobInterfaceTestImpl::backupAddressBookResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    auto job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setTestPath(mPathConfig);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportAddressbookJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportAddressbookJobInterfaceTestImpl::info);
    job->start();
}

#include "moc_exportaddressbookjobinterfacetestimpl.cpp"
