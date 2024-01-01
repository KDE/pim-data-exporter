/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportnotesjobinterfaceimpl.h"
#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"

#include <KConfigGroup>
#include <KZip>
#include <MailCommon/MailUtil>

#include <QStandardPaths>

ExportNotesJobInterfaceImpl::ExportNotesJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportNotesJobInterfaceImpl::~ExportNotesJobInterfaceImpl() = default;

QList<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

void ExportNotesJobInterfaceImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterImpl resourceImpl;
    resourceImpl.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}

Akonadi::Collection::Id ExportNotesJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportNotesJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    auto resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceNoteArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportNotesJobInterfaceImpl::emitError);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportNotesJobInterfaceImpl::emitInfo);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
    resourceJob->start();
}

QString ExportNotesJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}

QString ExportNotesJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString
ExportNotesJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}

#include "moc_exportnotesjobinterfaceimpl.cpp"
