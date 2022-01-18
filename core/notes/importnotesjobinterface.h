/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ImportNotesJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportNotesJobInterface() override;

    void start() override;

protected:
    Q_REQUIRED_RESULT virtual QString configLocation() const;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    Q_REQUIRED_RESULT virtual QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;
    void slotNextStep() override;
    Q_REQUIRED_RESULT QString installConfigLocation() const;

private:
    Q_REQUIRED_RESULT bool isAConfigFile(const QString &name) const override;
    void importKNoteGlobalSettings(const KArchiveFile *kmailsnippet, const QString &configrc, const QString &filename, const QString &prefix);
    void restoreConfig();
    void restoreData();
    void restoreResources();
};
