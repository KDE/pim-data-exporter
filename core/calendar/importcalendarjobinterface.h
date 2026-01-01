/*
   SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] virtual QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;

    [[nodiscard]] virtual QString configLocation() const;
    [[nodiscard]] QString applicationName() const override;

private:
    [[nodiscard]] bool isAConfigFile(const QString &name) const override;
    void importkorganizerConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void importeventViewConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void importKalendarConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void restoreResources();
    void restoreConfig();
    void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings) override;
    [[nodiscard]] QString installConfigLocation() const;
    void convertResourceColors(const KSharedConfig::Ptr &config);
};
