/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include "resourceconverterbase.h"
class PIMDATAEXPORTER_EXPORT ResourceConverterImpl : public ResourceConverterBase
{
public:
    ResourceConverterImpl();
    ~ResourceConverterImpl() override;
    [[nodiscard]] QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString installDefaultDirectory() override;

    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
};
