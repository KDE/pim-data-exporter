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
#include "exportresourcearchivejob.h"
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

void ExportMailJobInterfaceImpl::exportArchiveResource()
{
    QTimer::singleShot(0, this, &ExportMailJobInterfaceImpl::slotWriteNextArchiveResource);
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

QVector<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
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

QString ExportMailJobInterfaceImpl::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    return Utils::storeResources(archive, identifier, path);
}

QString ExportMailJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}
