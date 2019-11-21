/*
   Copyright (C) 2015-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
