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

#include "pimdataexporterconsole.h"
#include "pimdatabackuprestore.h"
#include "pimdataexportconsole_debug.h"
#include "loginfile.h"
#include "loginfo.h"
#include "xml/templateselection.h"

#include <QTimer>
#include <pimdataexporterkernel.h>
#include <MailCommon/MailKernel>
#include <MailCommon/FilterManager>

PimDataExporterConsole::PimDataExporterConsole(QObject *parent)
    : QObject(parent)
    , mPimDataBackupRestore(new PimDataBackupRestore(this))
    , mLogInFile(nullptr)
    , mLogInfo(new LogInfo(this))
    , mMode(Import)
    , mInProgress(false)
{
    //Initialize filtermanager
    (void)MailCommon::FilterManager::instance();
    PimDataExporterKernel *kernel = new PimDataExporterKernel(this);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget

    initializeLogInFile();
    //TODO initialize akonadi server
}

PimDataExporterConsole::~PimDataExporterConsole()
{
}

void PimDataExporterConsole::initializeLogInFile()
{
    connect(mPimDataBackupRestore, &PimDataBackupRestore::addEndLine, this, &PimDataExporterConsole::slotAddEndLine);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::addError, this, &PimDataExporterConsole::slotAddError);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::addInfo, this, &PimDataExporterConsole::slotAddInfo);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::addTitle, this, &PimDataExporterConsole::slotAddTitle);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::jobFinished, this, &PimDataExporterConsole::slotJobFinished);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::backupDone, this, &PimDataExporterConsole::slotBackupDone);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::jobFailed, this, &PimDataExporterConsole::slotJobFailed);
    connect(mPimDataBackupRestore, &PimDataBackupRestore::restoreDone, this, &PimDataExporterConsole::slotRestoreDone);
}

void PimDataExporterConsole::closeLogFile()
{
    delete mLogInFile;
    mLogInFile = nullptr;
}

void PimDataExporterConsole::slotRestoreDone()
{
    qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Restore Done";
    closeLogFile();
    QTimer::singleShot(0, this, &PimDataExporterConsole::finished);
}

void PimDataExporterConsole::slotJobFailed()
{
    qCWarning(PIMDATAEXPORTERCONSOLE_LOG) << "job failed";
    closeLogFile();
    mPimDataBackupRestore->closeArchive();
}

void PimDataExporterConsole::slotBackupDone()
{
    qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Backup Done";
    closeLogFile();
    QTimer::singleShot(0, this, &PimDataExporterConsole::finished);
}

void PimDataExporterConsole::slotJobFinished()
{
    qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "job finished";
    mPimDataBackupRestore->nextStep();
}

void PimDataExporterConsole::slotAddEndLine()
{
    if (mLogInFile) {
        mLogInFile->addEndLine();
    }
    mLogInfo->addEndLineLogEntry();
}

void PimDataExporterConsole::slotAddError(const QString &message)
{
    if (mLogInFile) {
        mLogInFile->addError(message);
    }
    mLogInfo->addErrorLogEntry(message);
}

void PimDataExporterConsole::slotAddInfo(const QString &message)
{
    if (mLogInFile) {
        mLogInFile->addInfo(message);
    }
    mLogInfo->addInfoLogEntry(message);
}

void PimDataExporterConsole::slotAddTitle(const QString &message)
{
    if (mLogInFile) {
        mLogInFile->addTitle(message);
    }
    mLogInfo->addTitleLogEntry(message);
}

QString PimDataExporterConsole::importExportFileName() const
{
    return mImportExportFileName;
}

void PimDataExporterConsole::setImportExportFileName(const QString &filename)
{
    if (mInProgress) {
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Already in progress. We can't change it.";
        return;
    }
    mImportExportFileName = filename;
}

void PimDataExporterConsole::start()
{
    //Load template if necessary
    if (!mTemplateFileName.isEmpty()) {
        TemplateSelection selection;
        const QHash<Utils::AppsType, Utils::importExportParameters> templateElements = selection.loadTemplate(mTemplateFileName);
        mPimDataBackupRestore->setStoredParameters(templateElements);
    }
    switch (mMode) {
    case Import:
        if (!mPimDataBackupRestore->restoreStart(mImportExportFileName)) {
            qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Unable to start restore.";
            QTimer::singleShot(0, this, &PimDataExporterConsole::finished);
        }
        break;
    case Export:
        if (!mPimDataBackupRestore->backupStart(mImportExportFileName)) {
            qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Unable to start backup.";
            QTimer::singleShot(0, this, &PimDataExporterConsole::finished);
        }
        break;
    }
}

PimDataExporterConsole::Mode PimDataExporterConsole::mode() const
{
    return mMode;
}

void PimDataExporterConsole::setMode(Mode mode)
{
    if (mInProgress) {
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Already in progress. We can't change it.";
        return;
    }
    mMode = mode;
}

void PimDataExporterConsole::setLogFileName(const QString &logFileName)
{
    if (mInProgress) {
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Already in progress. We can't change it.";
        return;
    }
    if (!mLogInFile) {
        mLogInFile = new LogInFile(this);
    }
    mLogInFile->setFileName(logFileName);
}

void PimDataExporterConsole::setTemplateFileName(const QString &templateFileName)
{
    if (mInProgress) {
        qCDebug(PIMDATAEXPORTERCONSOLE_LOG) << "Already in progress. We can't change it.";
        return;
    }
    mTemplateFileName = templateFileName;
}
