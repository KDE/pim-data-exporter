/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importalarmjobinterface.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ImportAlarmJobInterfaceImpl : public ImportAlarmJobInterface
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAlarmJobInterfaceImpl() override;

protected:
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    void synchronizeResource(const QStringList &lst) override;
    [[nodiscard]] QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
};
