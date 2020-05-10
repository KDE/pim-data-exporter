/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "exportnotesjobinterfaceimpl.h"
#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"

#include <MailCommon/MailUtil>
#include <KLocalizedString>
#include <KZip>
#include <KConfigGroup>

#include <QDir>
#include <QTimer>
#include <QStandardPaths>

ExportNotesJobInterfaceImpl::ExportNotesJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportNotesJobInterfaceImpl::~ExportNotesJobInterfaceImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

void ExportNotesJobInterfaceImpl::exportArchiveResource()
{
    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotWriteNextArchiveResource);
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
    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceNoteArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportNotesJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportNotesJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
    resourceJob->start();
}

QString ExportNotesJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}
