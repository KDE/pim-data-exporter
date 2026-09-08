/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejob.h"
using namespace Qt::Literals::StringLiterals;

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
    for (const auto &[key, value] : lstAttributeInfo.asKeyValueRange()) {
        KConfigGroup attributeGroup = conf.group(key);

        if (!value.displayAttribute.isEmpty()) {
            attributeGroup.writeEntry(u"Display"_s, value.displayAttribute);
        }
        if (!value.expireAttribute.isEmpty()) {
            attributeGroup.writeEntry(u"Expire"_s, value.expireAttribute);
        }
        if (!value.favoriteAttribute.isEmpty()) {
            attributeGroup.writeEntry(u"Favorite"_s, value.favoriteAttribute);
        }
        if (!value.folderAttribute.isEmpty()) {
            attributeGroup.writeEntry(u"Folder"_s, value.folderAttribute);
        }
    }
    conf.sync();
    tmp.close();

    const bool fileAdded = mArchive->addLocalFile(tmp.fileName(), Utils::configsPath() + u"mailfolderattributes"_s);
    if (fileAdded) {
        Q_EMIT successed();
    } else {
        Q_EMIT failed();
    }
    deleteLater();
}

#include "moc_exportmailfolderattributejob.cpp"
