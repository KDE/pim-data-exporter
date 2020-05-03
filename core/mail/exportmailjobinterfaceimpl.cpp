/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "exportmailjobinterfaceimpl.h"
#include "importexportmailutil.h"

#include <MailCommon/MailUtil>
#include <MailCommon/FilterManager>
#include <KMime/Message>
#include "importexportprogressindicatorbase.h"

#include <AkonadiCore/AgentManager>
#include <AkonadiCore/Collection>

#include <MailTransport/TransportManager>

#include <KZip>
#include <KLocalizedString>
#include <QTemporaryFile>

#include "pimdataexportcore_debug.h"

#include <QFile>
#include <QDir>
#include <QTimer>
#include <QStandardPaths>
#include <exportresourcearchivejob.h>
#include <QRegularExpression>
#include "resourceconverterimpl.h"

#include <KIdentityManagement/IdentityManager>
#include <KIdentityManagement/Identity>

ExportMailJobInterfaceImpl::ExportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportMailJobInterfaceImpl::~ExportMailJobInterfaceImpl()
{
}

void ExportMailJobInterfaceImpl::slotMailsJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    exportArchiveResource();
}

void ExportMailJobInterfaceImpl::exportArchiveResource()
{
    QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotWriteNextArchiveResource);
}

void ExportMailJobInterfaceImpl::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        const Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QStringList capabilities(agent.type().capabilities());
        if (agent.type().mimeTypes().contains(KMime::Message::mimeType())) {
            if (capabilities.contains(QLatin1String("Resource"))
                && !capabilities.contains(QLatin1String("Virtual"))
                && !capabilities.contains(QLatin1String("MailTransport"))) {
                const QString identifier = agent.identifier();
                if (identifier.contains(QLatin1String("akonadi_maildir_resource_"))
                    || identifier.contains(QLatin1String("akonadi_mixedmaildir_resource_"))) {
                    const QString archivePath = Utils::mailsPath() + identifier + QLatin1Char('/');
                    ResourceConverterImpl converter;
                    const QString url = converter.resourcePath(identifier);
                    if (!mAgentPaths.contains(url)) {
                        if (!url.isEmpty()) {
                            mAgentPaths << url;
                            exportResourceToArchive(archivePath, url, identifier);
                        } else {
                            qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                            QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
                        }
                    } else {
                        QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
                    }
                } else if (identifier.contains(QLatin1String("akonadi_mbox_resource_"))) {
                    backupResourceFile(identifier, Utils::addressbookPath());
                    QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
                } else {
                    QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
            }
        } else {
            QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
        }
    } else {
        QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotCheckBackupResources);
    }
}

void ExportMailJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(QStringLiteral("mail.zip"));
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportMailJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportMailJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
    connect(this, &ExportMailJobInterfaceImpl::taskCanceled, resourceJob, &ExportResourceArchiveJob::slotTaskCanceled);
    resourceJob->start();
}

void ExportMailJobInterfaceImpl::backupResources()
{
    setProgressDialogLabel(i18n("Backing up resources..."));

    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    for (const Akonadi::AgentInstance &agent : list) {
        const QStringList capabilities(agent.type().capabilities());
        if (agent.type().mimeTypes().contains(KMime::Message::mimeType())) {
            if (capabilities.contains(QLatin1String("Resource"))
                && !capabilities.contains(QLatin1String("Virtual"))
                && !capabilities.contains(QLatin1String("MailTransport"))) {
                const QString identifier = agent.identifier();
                //Store just pop3/imap/kolab/gmail account. Store other config when we copy data.
                if (identifier.contains(QLatin1String("pop3")) || identifier.contains(QLatin1String("imap"))
                    || identifier.contains(QLatin1String("_kolab_")) || identifier.contains(QLatin1String("_gmail_"))) {
                    const QString errorStr = Utils::storeResources(archive(), identifier, Utils::resourcesPath());
                    if (!errorStr.isEmpty()) {
                        Q_EMIT error(errorStr);
                    }
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " resource \"" << identifier << "\" will not store";
                }
            }
        }
    }

    Q_EMIT info(i18n("Resources backup done."));
}

Akonadi::Collection::Id ExportMailJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportMailJobInterfaceImpl::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    ResourceConverterImpl converter;
    converter.convertCollectionIdsToRealPath(group, currentKey, prefixCollection);
}

void ExportMailJobInterfaceImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionToRealPath(group, currentKey);
}

void ExportMailJobInterfaceImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionListToRealPath(group, currentKey);
}

QVector<MailCommon::MailFilter *> ExportMailJobInterfaceImpl::filters()
{
    return MailCommon::FilterManager::instance()->filters();
}


QString ExportMailJobInterfaceImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterImpl converter;
    return converter.convertToFullCollectionPath(collectionValue);
}
