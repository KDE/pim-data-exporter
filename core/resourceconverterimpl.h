/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "resourceconverterbase.h"
#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT ResourceConverterImpl : public ResourceConverterBase
{
public:
    ResourceConverterImpl();
    ~ResourceConverterImpl() override;
    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString installDefaultDirectory() override;

    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
};

