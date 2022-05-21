/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"
#include "utils.h"

#include <KArchive>
#include <KArchiveDirectory>
#include <KArchiveFile>

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
#if 0
        copyArchiveFileTo(file, destDirectory);
        const QString filename(file->name());
        const QString agentResourceFileName = destDirectory + QLatin1Char('/') + filename;
        resourceName = Utils::akonadiAgentName(agentResourceFileName);
#endif
    }
    applyAttributes();
}

void ImportMailFolderAttributeJob::restoreFileFolderAttribute()
{
    // TODO
    Q_EMIT successed();
    deleteLater();
}
