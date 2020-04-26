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

#include "exportcalendarjobinterfaceimpl.h"
#include <MailCommon/MailUtil>
#include <AkonadiCore/AgentManager>

#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QColor>
#include "resourceconverterimpl.h"

#include <QStandardPaths>
#include <exportresourcearchivejob.h>

ExportCalendarJobInterfaceImpl::ExportCalendarJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportCalendarJobInterfaceImpl::~ExportCalendarJobInterfaceImpl()
{
}

void ExportCalendarJobInterfaceImpl::exportArchiveResource()
{
    QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotWriteNextArchiveResource);
}

void ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotWriteNextArchiveResource);
}

void ExportCalendarJobInterfaceImpl::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        const Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QString identifier = agent.identifier();
        if (identifier.contains(QLatin1String("akonadi_icaldir_resource_"))) {
            const QString archivePath = Utils::calendarPath() + identifier + QLatin1Char('/');

            ResourceConverterImpl converter;
            const QString url = converter.resourcePath(identifier);
            if (!mAgentPaths.contains(url)) {
                mAgentPaths << url;
                if (!url.isEmpty()) {
                    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
                    resourceJob->setArchivePath(archivePath);
                    resourceJob->setUrl(url);
                    resourceJob->setIdentifier(identifier);
                    resourceJob->setArchive(archive());
                    resourceJob->setArchiveName(QStringLiteral("calendar.zip"));
                    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportCalendarJobInterfaceImpl::error);
                    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportCalendarJobInterfaceImpl::info);
                    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
                    resourceJob->start();
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
            }
        } else if (identifier.contains(QLatin1String("akonadi_ical_resource_"))) {
            backupResourceFile(agent, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportCalendarJobInterfaceImpl::slotCheckBackupConfig);
    }
}

