/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importakregatorjobinterfacetestimpl.h"

ImportAkregatorJobInterfaceTestImpl::ImportAkregatorJobInterfaceTestImpl(QObject *parent,
                                                                         Utils::StoredTypes typeSelected,
                                                                         ArchiveStorage *archiveStorage,
                                                                         int numberOfStep)
    : ImportAkregatorJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAkregatorJobInterfaceTestImpl::~ImportAkregatorJobInterfaceTestImpl() = default;

Akonadi::Collection::Id ImportAkregatorJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    Q_UNUSED(path);
    Q_UNREACHABLE();
    return -1;
}

QString ImportAkregatorJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNUSED(overwriteResources);
    Q_UNUSED(resourceConfig);
    Q_UNUSED(storePath);
    Q_UNREACHABLE();
    return {};
}
