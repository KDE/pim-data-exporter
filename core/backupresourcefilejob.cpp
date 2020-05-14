/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "backupresourcefilejob.h"
#include "resourceconverterimpl.h"
#include "storeresourcejob.h"
#include "pimdataexportcore_debug.h"

#include <KLocalizedString>

#include <QFileInfo>

BackupResourceFileJob::BackupResourceFileJob(QObject *parent)
    : QObject(parent)
{

}

BackupResourceFileJob::~BackupResourceFileJob()
{

}

void BackupResourceFileJob::start()
{
    if (!canStart()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to start backupResourceFileJob";
        deleteLater();
        return;
    }

    const QString archivePath = mDefaultPath + mIdentifier + QLatin1Char('/');

    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(mIdentifier);
    if (!url.isEmpty()) {
        QFileInfo fi(url);
        QString filename = fi.fileName();
        const bool fileAdded = mZip->addLocalFile(url, archivePath + filename);
        if (fileAdded) {
            Q_EMIT info(i18n("\"%1\" was backed up.", filename));

            StoreResourceJob *job = new StoreResourceJob(this);
            connect(job, &StoreResourceJob::error, this, &BackupResourceFileJob::error);
            connect(job, &StoreResourceJob::info, this, &BackupResourceFileJob::info);
            job->setArchivePath(archivePath);
            job->setZip(mZip);
            job->setIdentifier(mIdentifier);
            job->start();
        } else {
            Q_EMIT error(i18n("\"%1\" file cannot be added to backup file.", filename));
        }
    }
    deleteLater();
}

bool BackupResourceFileJob::canStart() const
{
    return mZip && !mDefaultPath.isEmpty() && !mIdentifier.isEmpty();
}

QString BackupResourceFileJob::identifier() const
{
    return mIdentifier;
}

void BackupResourceFileJob::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

KZip *BackupResourceFileJob::zip() const
{
    return mZip;
}

void BackupResourceFileJob::setZip(KZip *zip)
{
    mZip = zip;
}

QString BackupResourceFileJob::defaultPath() const
{
    return mDefaultPath;
}

void BackupResourceFileJob::setDefaultPath(const QString &defaultPath)
{
    mDefaultPath = defaultPath;
}
