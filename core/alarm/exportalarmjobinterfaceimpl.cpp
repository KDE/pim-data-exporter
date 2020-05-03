/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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

#include "exportalarmjobinterfaceimpl.h"

#include <AkonadiCore/AgentManager>

#include <KLocalizedString>
#include <MailCommon/MailUtil>

#include <QTemporaryFile>
#include <QTimer>
#include <KConfigGroup>

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <exportresourcearchivejob.h>
#include "resourceconverterimpl.h"

ExportAlarmJobInterfaceImpl::ExportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAlarmJobInterfaceImpl::~ExportAlarmJobInterfaceImpl()
{
}

void ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotWriteNextArchiveResource);
}

void ExportAlarmJobInterfaceImpl::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        const Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QString identifier = agent.identifier();
        if (identifier.contains(QLatin1String("akonadi_kalarm_dir_resource_"))) {
            const QString archivePath = Utils::alarmPath() + identifier + QLatin1Char('/');

            ResourceConverterImpl converter;
            const QString url = converter.resourcePath(identifier);
            if (!mAgentPaths.contains(url)) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
                    resourceJob->setArchivePath(archivePath);
                    resourceJob->setUrl(url);
                    resourceJob->setIdentifier(identifier);
                    resourceJob->setArchive(archive());
                    resourceJob->setArchiveName(QStringLiteral("alarm.zip"));
                    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAlarmJobInterfaceImpl::error);
                    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAlarmJobInterfaceImpl::info);
                    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
                    resourceJob->start();
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
            }
        } else if (identifier.contains(QLatin1String("akonadi_kalarm_resource_"))) {
            backupResourceFile(agent, Utils::alarmPath());
            QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotCheckBackupConfig);
    }
}

void ExportAlarmJobInterfaceImpl::exportArchiveResource()
{
    QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotWriteNextArchiveResource);
}

Akonadi::Collection::Id ExportAlarmJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}
