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

#ifndef RESOURCECONVERTERTEST_H
#define RESOURCECONVERTERTEST_H

#include "resourceconverterbase.h"
#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT ResourceConverterTest : public ResourceConverterBase
{
public:
    ResourceConverterTest();
    ~ResourceConverterTest() override;
    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;

    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;

    Q_REQUIRED_RESULT QString changeResourcePath(QString url) const override;

    void setTestPath(const QString &testPath);

    Q_REQUIRED_RESULT QString installDefaultDirectory() override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
private:
    Q_REQUIRED_RESULT QString prefixStr() const;
    QString mTestPath;
};

#endif // RESOURCECONVERTERTEST_H
