/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatabackupthread.h"
#include "pimdataexportcore_debug.h"

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

PimDataBackupThread::PimDataBackupThread(KZip *zip, const QString &url, const QString &archivePath, const QString &archivename, QObject *parent)
    : QThread(parent)
    , mUrl(url)
    , mArchivePath(archivePath)
    , mArchiveName(archivename)
    , mZip(zip)
{
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " PimDataBackupThread::PimDataBackupThread" << this;
}

PimDataBackupThread::~PimDataBackupThread()
{
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " PimDataBackupThread::~PimDataBackupThread()" << this;
}

void PimDataBackupThread::run()
{
    QTemporaryFile tmp;
    tmp.open();
    KZip *archiveFile = new KZip(tmp.fileName());
    archiveFile->setCompression(KZip::NoCompression);
    bool result = archiveFile->open(QIODevice::WriteOnly);
    if (!result) {
        Q_EMIT error(i18n("Impossible to open archive file."));
        Q_EMIT terminated(false);
        delete archiveFile;
        return;
    }
    const bool vcarddirAdded = archiveFile->addLocalDirectory(mUrl, QString());
    if (!vcarddirAdded) {
        Q_EMIT error(i18n("Impossible to backup \"%1\".", mUrl));
        Q_EMIT terminated(false);
        delete archiveFile;
        return;
    }
    archiveFile->close();
    tmp.close();

    const bool fileAdded = mZip->addLocalFile(tmp.fileName(), mArchivePath + mArchiveName);
    if (fileAdded) {
        Q_EMIT info(i18n("\"%1\" was backed up.", mUrl));
    } else {
        Q_EMIT error(i18n("\"%1\" file cannot be added to backup file.", mUrl));
    }
    delete archiveFile;
    Q_EMIT terminated(fileAdded);
}
