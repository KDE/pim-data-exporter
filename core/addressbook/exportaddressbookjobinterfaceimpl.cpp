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

#include "exportaddressbookjobinterfaceimpl.h"
#include "exportresourcearchivejob.h"

#include <AkonadiCore/AgentManager>
#include <MailCommon/MailUtil>
#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QDir>
#include <QStandardPaths>
#include <QTimer>
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

void ExportAddressbookJobInterfaceImpl::exportArchiveResource()
{
    mAkonadiInstanceInfo = listOfResource();
    QTimer::singleShot(0, this, &ExportAddressbookJobInterfaceImpl::slotWriteNextArchiveResource);
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
    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(QStringLiteral("addressbook.zip"));
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAddressbookJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAddressbookJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAddressbookJobInterfaceImpl::slotAddressbookJobTerminated);
    resourceJob->start();
}
