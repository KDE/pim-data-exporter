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

#include "importaddressbookjobinterfaceimpl.h"
#include "archivestorage.h"
#include <MailCommon/MailUtil>
#include <PimCommonAkonadi/CreateResource>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KZip>
#include <KArchiveEntry>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "resourceconverterimpl.h"

ImportAddressbookJobInterfaceImpl::ImportAddressbookJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAddressbookJobInterfaceImpl::~ImportAddressbookJobInterfaceImpl()
{
}

Akonadi::Collection::Id ImportAddressbookJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ImportAddressbookJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    //It's maildir support. Need to add support
    startSynchronizeResources(lst);
}

QString ImportAddressbookJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

QString ImportAddressbookJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}
