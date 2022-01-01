/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportakregatorjobinterfacetestimpl.h"

ExportAkregatorJobInterfaceTestImpl::ExportAkregatorJobInterfaceTestImpl(QObject *parent,
                                                                         Utils::StoredTypes typeSelected,
                                                                         ArchiveStorage *archiveStorage,
                                                                         int numberOfStep)
    : ExportAkregatorJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAkregatorJobInterfaceTestImpl::~ExportAkregatorJobInterfaceTestImpl()
{
}

QString ExportAkregatorJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNREACHABLE();
    return {};
}

QString ExportAkregatorJobInterfaceTestImpl::createResource(const QString &resources,
                                                            const QString &name,
                                                            const QMap<QString, QVariant> &settings,
                                                            bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}
