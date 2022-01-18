/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT QString agentFileName(const QString &filename);
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString());
    Q_REQUIRED_RESULT QString resourcePath(const QString &agentIdentifier, const QString &defaultPath = QString());
    Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData);
    Q_REQUIRED_RESULT QString resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath = QString());
    Q_REQUIRED_RESULT virtual QString changeResourcePath(QString url) const;

    Q_REQUIRED_RESULT virtual QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    Q_REQUIRED_RESULT virtual Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path);
    Q_REQUIRED_RESULT virtual QString installDefaultDirectory() = 0;
    virtual QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) = 0;
};
