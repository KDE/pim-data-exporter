/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportaddressbookjobinterfaceimpl.h"
#include "exportresourcearchivejob.h"

#include <AkonadiCore/AgentManager>
#include <MailCommon/MailUtil>

#include <KConfigGroup>

#include <QStandardPaths>
#include "resourceconverterimpl.h"

ExportAddressbookJobInterfaceImpl::ExportAddressbookJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAddressbookJobInterfaceImpl::~ExportAddressbookJobInterfaceImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportAddressbookJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

void ExportAddressbookJobInterfaceImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionToRealPath(group, currentKey);
}

void ExportAddressbookJobInterfaceImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionListToRealPath(group, currentKey);
}

Akonadi::Collection::Id ExportAddressbookJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportAddressbookJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    auto *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceAddressbookArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAddressbookJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAddressbookJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAddressbookJobInterfaceImpl::slotAddressbookJobTerminated);
    resourceJob->start();
}

QString ExportAddressbookJobInterfaceImpl::resourcePath(const QString &agentIdentifier, const QString &defaultPath) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(agentIdentifier, defaultPath);
    return url;
}

void ExportAddressbookJobInterfaceImpl::backupAddressBookResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    backupResourceFile(agentIdentifier, defaultPath);
}

QString ExportAddressbookJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ExportAddressbookJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}
