/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupresourcefilejobbase.h"
#include "storeresourcejob.h"
#include "pimdataexportcore_debug.h"

#include <KLocalizedString>

#include <QFileInfo>

BackupResourceFileJobBase::BackupResourceFileJobBase(QObject *parent)
    : QObject(parent)
{
}

BackupResourceFileJobBase::~BackupResourceFileJobBase()
{
}

void BackupResourceFileJobBase::storeResource(const QString &archivePath)
{
    auto job = new StoreResourceJob(this);
    connect(job, &StoreResourceJob::error, this, &BackupResourceFileJobBase::error);
    connect(job, &StoreResourceJob::info, this, &BackupResourceFileJobBase::info);
    job->setArchivePath(archivePath);
    job->setZip(mZip);
    job->setIdentifier(mIdentifier);
    job->start();
}

void BackupResourceFileJobBase::start()
{
    if (!canStart()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to start backupResourceFileJob";
        deleteLater();
        return;
    }

    const QString archivePath = mDefaultPath + mIdentifier + QLatin1Char('/');

    const QString url = resourcePath(mIdentifier);
    if (!url.isEmpty()) {
        QFileInfo fi(url);
        if (!fi.isFile()) {
            Q_EMIT info(i18n("\"%1\" is not a file.", url));
        } else {
            const QString filename = fi.fileName();
            const bool fileAdded = mZip->addLocalFile(url, archivePath + filename);
            if (fileAdded) {
                storeResource(archivePath);
                Q_EMIT info(i18n("\"%1\" was backed up.", filename));
            } else {
                Q_EMIT error(i18n("\"%1\" file cannot be added to backup file.", filename));
            }
        }
    } else {
        storeResource(archivePath);
        Q_EMIT info(i18n("\"%1\" was backed up.", mIdentifier));
    }
    deleteLater();
}

bool BackupResourceFileJobBase::canStart() const
{
    return mZip && !mDefaultPath.isEmpty() && !mIdentifier.isEmpty();
}

QString BackupResourceFileJobBase::identifier() const
{
    return mIdentifier;
}

void BackupResourceFileJobBase::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

KZip *BackupResourceFileJobBase::zip() const
{
    return mZip;
}

void BackupResourceFileJobBase::setZip(KZip *zip)
{
    mZip = zip;
}

QString BackupResourceFileJobBase::defaultPath() const
{
    return mDefaultPath;
}

void BackupResourceFileJobBase::setDefaultPath(const QString &defaultPath)
{
    mDefaultPath = defaultPath;
}
