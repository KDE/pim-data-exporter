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

#include "storeresourcejob.h"
#include "utils.h"
#include "pimdataexportcore_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <AkonadiCore/ServerManager>

#include <QFileInfo>

StoreResourceJob::StoreResourceJob(QObject *parent)
    : QObject(parent)
{

}

StoreResourceJob::~StoreResourceJob()
{

}

void StoreResourceJob::setArchivePath(const QString &archivePath)
{
    mArchivePath = archivePath;
}

void StoreResourceJob::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

void StoreResourceJob::setZip(KZip *zip)
{
    mZip = zip;
}

void StoreResourceJob::start()
{
    if (!mZip) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " mZip null it's a bug";
        deleteLater();
        return;
    }
    const QString errorStr = Utils::storeResources(mZip, mIdentifier, mArchivePath);
    if (!errorStr.isEmpty()) {
        Q_EMIT error(errorStr);
    }
    const QString url = Akonadi::ServerManager::agentConfigFilePath(mIdentifier);
    if (!url.isEmpty()) {
        const QFileInfo fi(url);
        const QString filename = fi.fileName();
        const bool fileAdded = mZip->addLocalFile(url, mArchivePath + filename);
        if (fileAdded) {
            Q_EMIT info(i18n("\"%1\" was backed up.", filename));
        } else {
            Q_EMIT error(i18n("\"%1\" file cannot be added to backup file.", filename));
        }
    }
    deleteLater();
}
