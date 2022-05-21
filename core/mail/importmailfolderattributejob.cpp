/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejob.h"
#include "importmailjobinterface.h"
#include "pimdataexportcore_debug.h"
#include "utils.h"

#include <KArchive>
#include <KArchiveDirectory>
#include <KArchiveFile>
#include <KConfigGroup>

ImportMailFolderAttributeJob::ImportMailFolderAttributeJob(QObject *parent)
    : QObject{parent}
{
}

ImportMailFolderAttributeJob::~ImportMailFolderAttributeJob()
{
}

bool ImportMailFolderAttributeJob::canStart() const
{
    return (mArchiveDirectory != nullptr) && !mExtractPath.isEmpty();
}

void ImportMailFolderAttributeJob::setArchiveDirectory(const KArchiveDirectory *zip)
{
    mArchiveDirectory = zip;
}

void ImportMailFolderAttributeJob::setExportInterface(ImportMailJobInterface *interface)
{
    mInterface = interface;
}

void ImportMailFolderAttributeJob::setExtractPath(const QString &extractPath)
{
    mExtractPath = extractPath;
}

void ImportMailFolderAttributeJob::start()
{
    if (!canStart()) {
        Q_EMIT failed();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Impossible to start job";
        deleteLater();
        return;
    }
    const KArchiveEntry *mailFolderAttributeFile = mArchiveDirectory->entry(Utils::configsPath() + QStringLiteral("mailfolderattributes"));
    if (mailFolderAttributeFile && mailFolderAttributeFile->isFile()) {
        const auto file = static_cast<const KArchiveFile *>(mailFolderAttributeFile);
        const QString destDirectory = mExtractPath + QLatin1Char('/') + Utils::resourcesPath();
        // qDebug() << " destDirectory " << destDirectory;
        mInterface->copyArchiveFileTo(file, destDirectory);
        const QString filename(file->name());
        const QString mailFolderAttributesFileName = destDirectory + QLatin1Char('/') + filename;
        KConfig conf(mailFolderAttributesFileName);
        const QString displayStr(QStringLiteral("Display"));
        const QString expireStr(QStringLiteral("Expire"));
        if (conf.hasGroup(displayStr)) {
            KConfigGroup group = conf.group(displayStr);
            const QStringList keyList = group.keyList();
            for (const QString &key : keyList) {
                const Akonadi::Collection::Id id = mInterface->convertPathToId(key);
                if (id != -1) {
                    const QByteArray displayBa = group.readEntry(key, QByteArray());
                    qDebug() << " displayBa " << displayBa;
                }
            }
        }
        if (conf.hasGroup(expireStr)) {
            KConfigGroup group = conf.group(expireStr);
            const QStringList keyList = group.keyList();
            for (const QString &key : keyList) {
                const Akonadi::Collection::Id id = mInterface->convertPathToId(key);
                if (id != -1) {
                    const QByteArray expireBa = group.readEntry(key, QByteArray());
                    qDebug() << " expireBa " << expireBa;
                }
            }
        }
    }
    applyAttributes();
}

void ImportMailFolderAttributeJob::restoreFileFolderAttribute()
{
    // TODO
    Q_EMIT successed();
    deleteLater();
}
