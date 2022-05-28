/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ImportAkregatorJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportAkregatorJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAkregatorJobInterface() override;

    void start() override;

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources,
                                             const QString &name,
                                             const QMap<QString, QVariant> &settings,
                                             bool synchronizeTree) override;
    Q_REQUIRED_RESULT QString applicationName() const override;

private:
    void slotNextStep() override;

private:
    void restoreConfig();
    void restoreData();
};
