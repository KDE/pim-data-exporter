/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importalarmjobinterfaceimpl.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>
#include <MailCommon/MailUtil>

#include <KArchive>
#include <KArchiveEntry>

#include <QStandardPaths>
#include "resourceconverterimpl.h"

ImportAlarmJobInterfaceImpl::ImportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAlarmJobInterfaceImpl::~ImportAlarmJobInterfaceImpl()
{
}

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

QString ImportAlarmJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
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
