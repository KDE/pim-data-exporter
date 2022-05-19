/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportresourcearchivejob.h"
#include "pimdataexportcore_debug.h"
#include "storeresourcejob.h"
#include "utils.h"
#include <KZip>
#include <pimdatabackupthread.h>

#include <Akonadi/ServerManager>

ExportResourceArchiveJob::ExportResourceArchiveJob(QObject *parent)
    : QObject(parent)
{
}

ExportResourceArchiveJob::~ExportResourceArchiveJob() = default;

void ExportResourceArchiveJob::setArchive(KZip *zip)
{
    mZip = zip;
}

void ExportResourceArchiveJob::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

void ExportResourceArchiveJob::setUrl(const QString &url)
{
    mUrl = url;
}

void ExportResourceArchiveJob::setArchivePath(const QString &archivePath)
{
    mArchivePath = archivePath;
}

void ExportResourceArchiveJob::setArchiveName(const QString &archiveName)
{
    mArchiveName = archiveName;
}

void ExportResourceArchiveJob::start()
{
    if (mZip) {
        mThread = new PimDataBackupThread(mZip, mUrl, mArchivePath, mArchiveName);
        connect(mThread, &PimDataBackupThread::error, this, &ExportResourceArchiveJob::error);
        connect(mThread, &PimDataBackupThread::info, this, &ExportResourceArchiveJob::info);
        connect(mThread, &PimDataBackupThread::terminated, this, &ExportResourceArchiveJob::slotTerminated);
        connect(mThread, &QThread::finished, mThread, &QObject::deleteLater);
        mThread->start();
    } else {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "zip not defined !";
        finished();
    }
}

void ExportResourceArchiveJob::slotTerminated(bool success)
{
    if (success) {
        auto job = new StoreResourceJob(this);
        connect(job, &StoreResourceJob::error, this, &ExportResourceArchiveJob::error);
        connect(job, &StoreResourceJob::info, this, &ExportResourceArchiveJob::info);
        job->setArchivePath(mArchivePath);
        job->setZip(mZip);
        job->setIdentifier(mIdentifier);
        job->start();
    }
    finished();
}

void ExportResourceArchiveJob::slotTaskCanceled()
{
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " void ExportResourceArchiveJob::slotTaskCanceled()";
    if (mThread) {
        mThread->exit();
    }
}

void ExportResourceArchiveJob::finished()
{
    Q_EMIT terminated();
    deleteLater();
}
