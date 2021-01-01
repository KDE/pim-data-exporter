/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "storeresourcejob.h"
#include "utils.h"
#include "pimdataexportcore_debug.h"
#include "resourceconverterimpl.h"
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

    ResourceConverterImpl converter;
    const QString errorStr = converter.storeResources(mZip, mIdentifier, mArchivePath);
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
