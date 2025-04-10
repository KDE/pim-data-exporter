/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class PimDataBackupRestore;
class LogInFile;
class LogInfo;
class PimDataExporterConsole : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        Import = 0,
        Export = 1
    };

    explicit PimDataExporterConsole(QObject *parent = nullptr);
    ~PimDataExporterConsole() override;

    [[nodiscard]] Mode mode() const;
    void setMode(Mode mode);

    void setLogFileName(const QString &logFileName);

    void setTemplateFileName(const QString &templateFileName);

    void start();

    [[nodiscard]] QString importExportFileName() const;
    void setImportExportFileName(const QString &importFileName);

Q_SIGNALS:
    void finished();

private:
    void slotAddEndLine();
    void slotAddError(const QString &message);
    void slotAddInfo(const QString &message);
    void slotAddTitle(const QString &message);
    void slotJobFailed();
    void slotBackupDone();
    void slotJobFinished();
    void slotRestoreDone();

    void initializeLogInFile();
    void closeLogFile();
    QString mTemplateFileName;
    QString mImportExportFileName;
    PimDataBackupRestore *const mPimDataBackupRestore;
    LogInFile *mLogInFile = nullptr;
    LogInfo *const mLogInfo;
    Mode mMode = Import;
    bool mInProgress = false;
};
