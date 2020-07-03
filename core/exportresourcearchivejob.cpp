/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "exportresourcearchivejob.h"
#include "utils.h"
#include "storeresourcejob.h"
#include "pimdataexportcore_debug.h"
#include <pimdatabackupthread.h>
#include <KZip>

#include <AkonadiCore/ServerManager>

ExportResourceArchiveJob::ExportResourceArchiveJob(QObject *parent)
    : QObject(parent)
{
}

ExportResourceArchiveJob::~ExportResourceArchiveJob()
{
}

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
        StoreResourceJob *job = new StoreResourceJob(this);
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
    //TODO
    if (mThread) {
        mThread->exit();
    }
}

void ExportResourceArchiveJob::finished()
{
    Q_EMIT terminated();
    deleteLater();
}
