/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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

ExportAkregatorJobInterfaceTestImpl::~ExportAkregatorJobInterfaceTestImpl() = default;

QString ExportAkregatorJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNUSED(overwriteResources);
    Q_UNUSED(resourceConfig);
    Q_UNUSED(storePath);
    Q_UNREACHABLE();
    return {};
}

QString ExportAkregatorJobInterfaceTestImpl::createResource(const QString &resources,
                                                            const QString &name,
                                                            const QMap<QString, QVariant> &settings,
                                                            bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(settings);
    Q_UNUSED(name);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}
