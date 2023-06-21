/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importnotesjobinterfaceimpl.h"
#include "archivestorage.h"
#include <MailCommon/MailUtil>
#include <PimCommonAkonadi/CreateResource>

#include <KArchive>

#include "resourceconverterimpl.h"
#include <QStandardPaths>

ImportNotesJobInterfaceImpl::ImportNotesJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
    initializeImportJob();
}

ImportNotesJobInterfaceImpl::~ImportNotesJobInterfaceImpl() = default;

QString ImportNotesJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

Akonadi::Collection::Id ImportNotesJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString
ImportNotesJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportNotesJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    // It's maildir support. Need to add support
    startSynchronizeResources(lst);
}

QString ImportNotesJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

#include "moc_importnotesjobinterfaceimpl.cpp"
