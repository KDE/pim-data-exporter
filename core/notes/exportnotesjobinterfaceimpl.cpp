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

#include <AkonadiCore/AgentManager>
#include <MailCommon/MailUtil>
#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>
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

void ExportNotesJobInterfaceImpl::slotNoteJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportNotesJobInterfaceImpl::slotWriteNextArchiveResource);
}

void ExportNotesJobInterfaceImpl::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QString identifier = agent.identifier();
        if (identifier.contains(QLatin1String("akonadi_akonotes_resource_"))) {
            const QString archivePath = Utils::notePath() + identifier + QLatin1Char('/');

            ResourceConverterImpl converter;
            QString url = converter.resourcePath(identifier);
            if (!mAgentPaths.contains(url) && QDir(url).exists()) {
                mAgentPaths << url;
                if (!url.isEmpty()) {
                    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
                    resourceJob->setArchivePath(archivePath);
                    resourceJob->setUrl(url);
                    resourceJob->setIdentifier(identifier);
                    resourceJob->setArchive(archive());
                    resourceJob->setArchiveName(QStringLiteral("notes.zip"));
                    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportNotesJobInterfaceImpl::error);
                    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportNotesJobInterfaceImpl::info);
                    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
                    resourceJob->start();
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
            }
        } else {
            QTimer::singleShot(0, this, &ExportNotesJobInterfaceImpl::slotNoteJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupConfig);
    }
}

void ExportNotesJobInterfaceImpl::exportArchiveResource()
{
    slotNoteJobTerminated();
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
