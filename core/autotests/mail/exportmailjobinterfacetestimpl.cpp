/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include "testbackupresourcefilejob.h"
#include <QDebug>

ExportMailJobInterfaceTestImpl::ExportMailJobInterfaceTestImpl(QObject *parent,
                                                               Utils::StoredTypes typeSelected,
                                                               ArchiveStorage *archiveStorage,
                                                               int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportMailJobInterfaceTestImpl::~ExportMailJobInterfaceTestImpl()
{
}

void ExportMailJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionIdsToRealPath(group, currentKey, prefixCollection);
}

Akonadi::Collection::Id ExportMailJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QVector<MailCommon::MailFilter *> ExportMailJobInterfaceTestImpl::filters()
{
    return {};
}

void ExportMailJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    // qDebug() << " void ExportMailJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)" <<
    // identifier;
    SaveResourceConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath,
                                               url,
                                               identifier,
                                               Utils::resourceMailArchiveName(),
                                               {QLatin1String("akonadi_maildir_resource_"), QLatin1String("akonadi_mixedmaildir_resource_")});
    slotMailsJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

QString ExportMailJobInterfaceTestImpl::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    return converter.storeResources(archive, identifier, path);
}

QString ExportMailJobInterfaceTestImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    return converter.convertToFullCollectionPath(collectionValue);
}

QString ExportMailJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportMailJobInterfaceTestImpl::backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    auto job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setTestPath(mPathConfig);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportMailJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportMailJobInterfaceTestImpl::info);
    job->start();
}

QString ExportMailJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString
ExportMailJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(settings);
    Q_UNUSED(name);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}

QVector<uint> ExportMailJobInterfaceTestImpl::listIdentityUoid() const
{
    return {10, 11, 12, 13, 14};
}

void ExportMailJobInterfaceTestImpl::exportFilters()
{
    const QString filename = mPathConfig + QLatin1String("filters");
    const bool fileAdded = archive()->addLocalFile(filename, Utils::configsPath() + QStringLiteral("filters"));
    if (fileAdded) {
        Q_EMIT info(QStringLiteral("Filters backup done."));
    } else {
        Q_EMIT error(QStringLiteral("Filters cannot be exported."));
    }
}
