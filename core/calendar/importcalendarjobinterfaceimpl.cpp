/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcalendarjobinterfaceimpl.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KArchiveDirectory>
#include <KArchiveEntry>

#include "pimdataexportcore_debug.h"

#include "resourceconverterimpl.h"
#include <MailCommon/MailUtil>
#include <QStandardPaths>

ImportCalendarJobInterfaceImpl::ImportCalendarJobInterfaceImpl(QObject *parent,
                                                               Utils::StoredTypes typeSelected,
                                                               ArchiveStorage *archiveStorage,
                                                               int numberOfStep)
    : ImportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportCalendarJobInterfaceImpl::~ImportCalendarJobInterfaceImpl()
{
}

QString ImportCalendarJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

Akonadi::Collection::Id ImportCalendarJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString
ImportCalendarJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportCalendarJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    startSynchronizeResources(lst);
}

QString ImportCalendarJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}
