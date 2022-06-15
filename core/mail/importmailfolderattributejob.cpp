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
    QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> mapAttributeInfo;
    if (mailFolderAttributeFile && mailFolderAttributeFile->isFile()) {
        const auto file = static_cast<const KArchiveFile *>(mailFolderAttributeFile);
        const QString destDirectory = mExtractPath + QLatin1Char('/') + Utils::resourcesPath();
        // qDebug() << " destDirectory " << destDirectory;
        mInterface->copyArchiveFileTo(file, destDirectory);
        const QString filename(file->name());
        const QString mailFolderAttributesFileName = destDirectory + QLatin1Char('/') + filename;
        KConfig conf(mailFolderAttributesFileName, KConfig::SimpleConfig);

        const QStringList groupList = conf.groupList();
        for (const QString &groupStr : groupList) {
            ImportExportMailUtil::AttributeInfo info;
            KConfigGroup group = conf.group(groupStr);
            // qDebug() << "groupStr  " << groupStr;
            const Akonadi::Collection::Id id = mInterface->convertPathToId(groupStr);
            const QString displayStr(QStringLiteral("Display"));
            if (group.hasKey(displayStr)) {
                info.displayAttribute = group.readEntry(displayStr, QByteArray());
            }
            const QString expireStr(QStringLiteral("Expire"));
            if (group.hasKey(expireStr)) {
                info.expireAttribute = group.readEntry(expireStr, QByteArray());
            }
            const QString favoriteStr(QStringLiteral("Favorite"));
            if (group.hasKey(favoriteStr)) {
                info.favoriteAttribute = group.readEntry(favoriteStr, QByteArray());
            }
            const QString folderStr(QStringLiteral("Folder"));
            if (group.hasKey(folderStr)) {
                info.folderAttribute = group.readEntry(folderStr, QByteArray());
            }
            // qDebug() << " identifier " << id << " info.expireAttribute " << info.expireAttribute << " info.displayAttribute " << info.displayAttribute;
            mapAttributeInfo.insert(id, info);
        }
    }
    applyAttributes(mapAttributeInfo);
}

void ImportMailFolderAttributeJob::restoreFileFolderAttribute()
{
    Q_EMIT successed();
    deleteLater();
}
