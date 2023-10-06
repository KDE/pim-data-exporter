/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportimportutil.h"

#include "alarm/importalarmjobinterface.h"

class ImportAlarmJobInterfaceTestImpl : public ImportAlarmJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAlarmJobInterfaceTestImpl() override;

    void setExistingPathConfig(const QString &path);

protected:
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    void synchronizeResource(const QStringList &lst) override;
    [[nodiscard]] QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    [[nodiscard]] QString configLocation() const override;

private:
    QString mExistingPathConfig;
};
