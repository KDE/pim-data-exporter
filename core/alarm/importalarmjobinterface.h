/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ImportAlarmJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAlarmJobInterface() override;

    void start() override;

protected:
    void slotNextStep() override;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    Q_REQUIRED_RESULT virtual QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;

    void restoreResources();
    Q_REQUIRED_RESULT virtual QString configLocation() const;
    Q_REQUIRED_RESULT QString applicationName() const override;

private:
    Q_REQUIRED_RESULT bool isAConfigFile(const QString &name) const override;
    void importkalarmConfig(const KArchiveFile *kalarmFile, const QString &kalarmrc, const QString &filename, const QString &prefix);
    void restoreConfig();
    Q_REQUIRED_RESULT QString installConfigLocation() const;
};
