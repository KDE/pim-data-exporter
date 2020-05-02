/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

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

#include "pimdatabackuprestore.h"
#include "archivestorage.h"
#include "importexportprogressindicatorbase.h"

#include "mail/exportmailjobinterfaceimpl.h"
#include "mail/importmailjobinterface.h"

#include "addressbook/exportaddressbookjobinterfaceimpl.h"
#include "addressbook/importaddressbookjobinterfaceimpl.h"

#include "calendar/importcalendarjobinterfaceimpl.h"
#include "calendar/exportcalendarjobinterfaceimpl.h"

#include "alarm/exportalarmjobinterfaceimpl.h"
#include "alarm/importalarmjobinterfaceimpl.h"

#include "notes/exportnotesjobinterfaceimpl.h"
#include "notes/importnotesjobinterfaceimpl.h"

#include "akregator/exportakregatorjobinterface.h"
#include "akregator/importakregatorjobinterface.h"

#include <KLocalizedString>
#include <QFile>

#include "pimdataexportcore_debug.h"
#include <QDateTime>
#include <QLocale>

PimDataBackupRestore::PimDataBackupRestore(QObject *parent)
    : QObject(parent)
{
}

PimDataBackupRestore::~PimDataBackupRestore()
{
    delete mImportExportData;
    mImportExportData = nullptr;
    QFile f(mExportedInfoFileName);
    if (f.exists()) {
        const bool removeFile = f.remove();
        if (!removeFile) {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "Impossible to remove exportedfile info" << mExportedInfoFileName;
        }
    }
}

void PimDataBackupRestore::setExportedInfoFileName(const QString &filename)
{
    mExportedInfoFileName = filename;
}

void PimDataBackupRestore::setStoredParameters(const QHash<Utils::AppsType, Utils::importExportParameters> &stored)
{
    mStored = stored;
}

bool PimDataBackupRestore::openArchive(const QString &filename, bool readWrite)
{
    mArchiveStorage = new ArchiveStorage(filename, this);
    if (!mArchiveStorage->openArchive(readWrite)) {
        delete mArchiveStorage;
        mArchiveStorage = nullptr;
        return false;
    }
    return true;
}

bool PimDataBackupRestore::backupStart(const QString &filename)
{
    if (mStored.isEmpty()) {
        addDate();
        Q_EMIT addInfo(i18n("No data selected."));
        Q_EMIT jobFailed();
        deleteLater();
        return false;
    }
    if (!openArchive(filename, true)) {
        Q_EMIT addError(i18n("Unable to open file \"%1\".", filename));
        Q_EMIT jobFailed();
        deleteLater();
        return false;
    }
    Q_EMIT updateActions(true);
    mAction = Backup;
    mStoreIterator = mStored.constBegin();
    addDate();
    Q_EMIT addInfo(i18n("Starting to backup data in \'%1\'", mArchiveStorage->filename()));
    Q_EMIT addEndLine();
    //Add version
    Utils::addVersion(mArchiveStorage->archive());
    //Add exported file info.
    //qDebug() << "mExportedInfoFileName" << mExportedInfoFileName;
    Utils::storeDataExportInfo(mArchiveStorage->archive());
    backupNextStep();
    return true;
}

void PimDataBackupRestore::nextStep()
{
    ++mStoreIterator;
    Q_EMIT addEndLine();
    switch (mAction) {
    case Backup:
        backupNextStep();
        break;
    case Restore:
        restoreNextStep();
    }
}

void PimDataBackupRestore::backupNextStep()
{
    if (mStoreIterator != mStored.constEnd()) {
        switch (mStoreIterator.key()) {
        case Utils::KMail:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportMailJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KAddressBook:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportAddressbookJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KAlarm:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportAlarmJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KOrganizer:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportCalendarJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KNotes:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportNotesJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::Akregator:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ExportAkregatorJobInterface(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::Unknown:
            break;
        }
    } else {
        backupFinished();
    }
}

void PimDataBackupRestore::closeArchive()
{
    if (mArchiveStorage) {
        mArchiveStorage->closeArchive();
        delete mArchiveStorage;
        mArchiveStorage = nullptr;
    }
    if (mImportExportData) {
        delete mImportExportData;
        mImportExportData = nullptr;
    }
    Q_EMIT updateActions(false);
}

void PimDataBackupRestore::backupFinished()
{
    Q_EMIT addInfo(i18n("Backup in \'%1\' done.", mArchiveStorage->filename()));
    //At the end
    closeArchive();
    Q_EMIT backupDone();
    deleteLater();
}

void PimDataBackupRestore::restoreNextStep()
{
    if (mStoreIterator != mStored.constEnd()) {
        switch (mStoreIterator.key()) {
        case Utils::KMail:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportMailJobInterface(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KAddressBook:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportAddressbookJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KAlarm:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportAlarmJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KOrganizer:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportCalendarJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::KNotes:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportNotesJobInterfaceImpl(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::Akregator:
            if (mStoreIterator.value().numberSteps != 0) {
                mImportExportData = new ImportAkregatorJobInterface(this, mStoreIterator.value().types, mArchiveStorage, mStoreIterator.value().numberSteps);
                executeJob();
            }
            break;
        case Utils::Unknown:
            break;
        }
    } else {
        restoreFinished();
    }
}

bool PimDataBackupRestore::continueToRestore()
{
    return true;
}

void PimDataBackupRestore::addDate()
{
    const QDateTime now = QDateTime::currentDateTime();
    Q_EMIT addInfo(QLatin1Char('[') + QLocale().toString((now), QLocale::ShortFormat) + QLatin1Char(']'));
}

bool PimDataBackupRestore::restoreStart(const QString &filename)
{
    if (mStored.isEmpty()) {
        addDate();
        Q_EMIT addInfo(i18n("No data selected."));
        Q_EMIT jobFailed();
        deleteLater();
        return false;
    }
    if (!openArchive(filename, false)) {
        Q_EMIT addError(i18n("Unable to open file \"%1\".", filename));
        Q_EMIT jobFailed();
        deleteLater();
        return false;
    }
    Q_EMIT updateActions(true);
    mAction = Restore;
    mStoreIterator = mStored.constBegin();
    const int version = Utils::archiveVersion(mArchiveStorage->archive());
    if (version > Utils::currentArchiveVersion()) {
        if (!continueToRestore()) {
            return false;
        }
    }
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " version " << version;
    AbstractImportExportJob::setArchiveVersion(version);

    addDate();

    Q_EMIT addInfo(i18n("Starting to restore data from \'%1\'", mArchiveStorage->filename()));
    Q_EMIT addEndLine();
    restoreNextStep();
    return true;
}

void PimDataBackupRestore::restoreFinished()
{
    Q_EMIT addInfo(i18n("Restoring data from \'%1\' done.", mArchiveStorage->filename()));
    //At the end
    mArchiveStorage->closeArchive();
    delete mArchiveStorage;
    mArchiveStorage = nullptr;
    delete mImportExportData;
    mImportExportData = nullptr;
    Q_EMIT restoreDone();
    deleteLater();
}

void PimDataBackupRestore::executeJob()
{
    addExportProgressIndicator();
    connect(mImportExportData, &AbstractImportExportJob::info, this, &PimDataBackupRestore::addInfo);
    connect(mImportExportData, &AbstractImportExportJob::error, this, &PimDataBackupRestore::addError);
    connect(mImportExportData, &AbstractImportExportJob::title, this, &PimDataBackupRestore::addTitle);
    connect(mImportExportData, &AbstractImportExportJob::endLine, this, &PimDataBackupRestore::addEndLine);
    connect(mImportExportData, &AbstractImportExportJob::jobFinished, this, &PimDataBackupRestore::jobFinished);
    connect(mImportExportData, &AbstractImportExportJob::needSynchronizeResource, this, &PimDataBackupRestore::needSyncResource);
    mImportExportData->start();
}

void PimDataBackupRestore::addExportProgressIndicator()
{
    mImportExportData->setImportExportProgressIndicator(new ImportExportProgressIndicatorBase(this));
}

void PimDataBackupRestore::slotJobFinished()
{
    ++mStoreIterator;
    Q_EMIT addEndLine();
    delete mImportExportData;
    mImportExportData = nullptr;
    switch (mAction) {
    case Backup:
        backupNextStep();
        break;
    case Restore:
        restoreNextStep();
        break;
    }
}
