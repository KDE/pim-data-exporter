/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTERCONSOLE_H
#define PIMDATAEXPORTERCONSOLE_H

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
    ~PimDataExporterConsole();

    Q_REQUIRED_RESULT Mode mode() const;
    void setMode(Mode mode);

    void setLogFileName(const QString &logFileName);

    void setTemplateFileName(const QString &templateFileName);

    void start();

    Q_REQUIRED_RESULT QString importExportFileName() const;
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
    PimDataBackupRestore *mPimDataBackupRestore = nullptr;
    LogInFile *mLogInFile = nullptr;
    LogInfo *mLogInfo = nullptr;
    Mode mMode = Import;
    bool mInProgress = false;
};

#endif // PIMDATAEXPORTERCONSOLE_H
