/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include <Akonadi/Collection>
#include <KSharedConfig>
#include <QString>
class KZip;
class PIMDATAEXPORTER_EXPORT ResourceConverterBase
{
public:
    ResourceConverterBase();
    virtual ~ResourceConverterBase();
    [[nodiscard]] QString agentFileName(const QString &filename);
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString());
    [[nodiscard]] QString resourcePath(const QString &agentIdentifier, const QString &defaultPath = QString());
    [[nodiscard]] QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData);
    [[nodiscard]] QString resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath = QString());
    [[nodiscard]] virtual QString changeResourcePath(QString url) const;

    [[nodiscard]] virtual QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    [[nodiscard]] virtual Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    [[nodiscard]] QString storeResources(KZip *archive, const QString &identifier, const QString &path);
    [[nodiscard]] virtual QString installDefaultDirectory() = 0;
    virtual QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) = 0;
};
