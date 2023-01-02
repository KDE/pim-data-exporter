/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importaddressbookjobinterfaceimpl.h"
#include "archivestorage.h"
#include <KArchive>
#include <KArchiveEntry>
#include <MailCommon/MailUtil>
#include <PimCommonAkonadi/CreateResource>

#include "resourceconverterimpl.h"
#include <QStandardPaths>

ImportAddressbookJobInterfaceImpl::ImportAddressbookJobInterfaceImpl(QObject *parent,
                                                                     Utils::StoredTypes typeSelected,
                                                                     ArchiveStorage *archiveStorage,
                                                                     int numberOfStep)
    : ImportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAddressbookJobInterfaceImpl::~ImportAddressbookJobInterfaceImpl() = default;

Akonadi::Collection::Id ImportAddressbookJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ImportAddressbookJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    // It's maildir support. Need to add support
    startSynchronizeResources(lst);
}

QString
ImportAddressbookJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

QString ImportAddressbookJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

QString ImportAddressbookJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}
