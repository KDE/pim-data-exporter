/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"
#include "utils.h"
#include <KConfigGroup>
#include <KZip>
#include <QTemporaryFile>

ExportMailFolderAttributeJob::ExportMailFolderAttributeJob(QObject *parent)
    : QObject{parent}
{
}

ExportMailFolderAttributeJob::~ExportMailFolderAttributeJob()
{
}

bool ExportMailFolderAttributeJob::canStart() const
{
    return (mArchive != nullptr);
}

void ExportMailFolderAttributeJob::start()
{
    if (!canStart()) {
        Q_EMIT failed();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Impossible to start job";
        deleteLater();
        return;
    }
    fetchAttributes();
}

void ExportMailFolderAttributeJob::setArchive(KZip *zip)
{
    mArchive = zip;
}

void ExportMailFolderAttributeJob::setExportInterface(ExportMailJobInterface *interface)
{
    mInterface = interface;
}

void ExportMailFolderAttributeJob::storeFileFolderAttribute(const QMap<QString, ImportExportMailUtil::AttributeInfo> &lstAttributeInfo)
{
    QTemporaryFile tmp;
    tmp.open();

    KConfig conf(tmp.fileName());
    QMapIterator<QString, ImportExportMailUtil::AttributeInfo> i(lstAttributeInfo);
    while (i.hasNext()) {
        i.next();
        KConfigGroup attributeGroup = conf.group(i.key());

        auto attr = i.value();
        if (!attr.displayAttribute.isEmpty()) {
            attributeGroup.writeEntry(QStringLiteral("Display"), attr.displayAttribute);
        }
        if (!attr.expireAttribute.isEmpty()) {
            attributeGroup.writeEntry(QStringLiteral("Expire"), attr.expireAttribute);
        }
        if (!attr.favoriteAttribute.isEmpty()) {
            attributeGroup.writeEntry(QStringLiteral("Favorite"), attr.favoriteAttribute);
        }
        if (!attr.folderAttribute.isEmpty()) {
            attributeGroup.writeEntry(QStringLiteral("Folder"), attr.folderAttribute);
        }
    }
    conf.sync();
    tmp.close();

    const bool fileAdded = mArchive->addLocalFile(tmp.fileName(), Utils::configsPath() + QStringLiteral("mailfolderattributes"));
    if (fileAdded) {
        Q_EMIT successed();
    } else {
        Q_EMIT failed();
    }
    deleteLater();
}

#include "moc_exportmailfolderattributejob.cpp"
