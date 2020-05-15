/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef RESOURCECONVERTERBASE_H
#define RESOURCECONVERTERBASE_H

#include "pimdataexporter_export.h"
#include <QString>
#include <KSharedConfig>
#include <AkonadiCore/Collection>
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
    virtual Q_REQUIRED_RESULT QString changeResourcePath(QString url) const;

    virtual Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    virtual Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path);
    virtual Q_REQUIRED_RESULT QString installDefaultDirectory() = 0;
    virtual QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) = 0;
};

#endif // RESOURCECONVERTER_H
