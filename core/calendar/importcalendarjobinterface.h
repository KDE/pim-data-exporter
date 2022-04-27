/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;
class KArchive;

class PIMDATAEXPORTER_TESTS_EXPORT ImportCalendarJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportCalendarJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportCalendarJobInterface() override;

    void start() override;

protected:
    void slotNextStep() override;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    Q_REQUIRED_RESULT virtual QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;

    Q_REQUIRED_RESULT virtual QString configLocation() const;

private:
    Q_REQUIRED_RESULT bool isAConfigFile(const QString &name) const override;
    void importkorganizerConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void importeventViewConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void importReminderAgentConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void importKalendarConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void restoreResources();
    void restoreConfig();
    void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings) override;
    Q_REQUIRED_RESULT QString installConfigLocation() const;
};
