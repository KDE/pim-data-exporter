/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESOURCECONVERTERIMPL_H
#define RESOURCECONVERTERIMPL_H

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

#endif // RESOURCECONVERTERIMPL_H
