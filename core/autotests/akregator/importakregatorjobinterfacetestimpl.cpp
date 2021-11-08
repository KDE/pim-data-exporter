/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

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

ImportAkregatorJobInterfaceTestImpl::~ImportAkregatorJobInterfaceTestImpl()
{
}

Akonadi::Collection::Id ImportAkregatorJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    Q_UNREACHABLE();
    return -1;
}

QString ImportAkregatorJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNREACHABLE();
    return {};
}
