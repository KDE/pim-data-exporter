/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESOURCECONVERTERTEST_H
#define RESOURCECONVERTERTEST_H

#include "resourceconverterbase.h"
#include "pimdataexporter_export.h"
class ResourceConverterTest : public ResourceConverterBase
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
