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

#ifndef RESOURCECONVERTER_H
#define RESOURCECONVERTER_H

#include "pimdataexporter_export.h"
#include <QString>
#include <KSharedConfig>
namespace Akonadi {
class AgentInstance;
}
class PIMDATAEXPORTER_EXPORT ResourceConverter
{
public:
    ResourceConverter();
    QString agentFileName(const QString &filename);
    QString resourcePath(const Akonadi::AgentInstance &agent, const QString &defaultPath = QString());
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey);
    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString());
    QString resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath);
    QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData);
};

#endif // RESOURCECONVERTER_H
