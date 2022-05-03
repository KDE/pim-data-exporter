/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportalarmjobinterfaceimpl.h"

#include <MailCommon/MailUtil>

#include <KConfigGroup>

#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"
#include <QStandardPaths>

ExportAlarmJobInterfaceImpl::ExportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAlarmJobInterfaceImpl::~ExportAlarmJobInterfaceImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportAlarmJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

Akonadi::Collection::Id ExportAlarmJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportAlarmJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    auto resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceAlarmArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAlarmJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAlarmJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
    resourceJob->start();
}

QString ExportAlarmJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportAlarmJobInterfaceImpl::backupAlarmResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    backupResourceFile(agentIdentifier, defaultPath);
}

QString ExportAlarmJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString
ExportAlarmJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}

void ExportAlarmJobInterfaceImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterImpl converter;
    converter.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}
