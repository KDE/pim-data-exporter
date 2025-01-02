/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importalarmjobinterfaceimpl.h"
#include "archivestorage.h"

#include <MailCommon/MailUtil>
#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KArchiveEntry>

#include "resourceconverterimpl.h"
#include <QStandardPaths>

ImportAlarmJobInterfaceImpl::ImportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAlarmJobInterfaceImpl::~ImportAlarmJobInterfaceImpl() = default;

Akonadi::Collection::Id ImportAlarmJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString ImportAlarmJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

QString
ImportAlarmJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

QString ImportAlarmJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ImportAlarmJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    startSynchronizeResources(lst);
}

#include "moc_importalarmjobinterfaceimpl.cpp"
