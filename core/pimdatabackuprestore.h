/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QHash>
#include "utils.h"
class AbstractImportExportJob;
class ArchiveStorage;

class PIMDATAEXPORTER_EXPORT PimDataBackupRestore : public QObject
{
    Q_OBJECT
public:
    explicit PimDataBackupRestore(QObject *parent = nullptr);
    ~PimDataBackupRestore() override;

    Q_REQUIRED_RESULT bool backupStart(const QString &filename);
    Q_REQUIRED_RESULT bool restoreStart(const QString &filename);

    void setStoredParameters(const QHash<Utils::AppsType, Utils::importExportParameters> &stored);

    void nextStep();
    void closeArchive();
    void setExportedInfoFileName(const QString &filename);

Q_SIGNALS:
    void addInfo(const QString &);
    void addEndLine();
    void updateActions(bool state);
    void addError(const QString &);
    void addTitle(const QString &);
    void jobFinished();
    void backupDone();
    void restoreDone();
    void jobFailed();
    void needSyncResource(const QString &name, const QString &identifier);

protected:
    virtual bool continueToRestore();

    virtual void addExportProgressIndicator();
    AbstractImportExportJob *mImportExportData = nullptr;
private:
    void slotJobFinished();
    enum Action {
        Backup,
        Restore
    };
    Q_REQUIRED_RESULT bool openArchive(const QString &filename, bool readWrite);
    void backupNextStep();
    void restoreNextStep();
    void backupFinished();
    void restoreFinished();
    void executeJob();
    void addDate();

    QString mExportedInfoFileName;
    QHash<Utils::AppsType, Utils::importExportParameters> mStored;
    QHash<Utils::AppsType, Utils::importExportParameters>::const_iterator mStoreIterator;
    Action mAction = Backup;
    ArchiveStorage *mArchiveStorage = nullptr;
};

