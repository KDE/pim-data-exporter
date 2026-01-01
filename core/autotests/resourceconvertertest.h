/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include "resourceconverterbase.h"
class ResourceConverterTest : public ResourceConverterBase
{
public:
    ResourceConverterTest();
    ~ResourceConverterTest() override;
    [[nodiscard]] QString convertToFullCollectionPath(const qlonglong collectionValue) override;

    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;

    [[nodiscard]] QString changeResourcePath(QString url) const override;

    void setTestPath(const QString &testPath);

    [[nodiscard]] QString installDefaultDirectory() override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;

private:
    [[nodiscard]] QString prefixStr() const;
    QString mTestPath;
};
