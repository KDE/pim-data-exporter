/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"
#include "utils.h"
#include <KConfigGroup>
#include <KZip>
#include <QTemporaryFile>
#include <QUrl>

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

void ExportMailFolderAttributeJob::storeFileFolderAttribute(const QMap<QString, AttributeInfo> &lstAttributeInfo)
{
    QTemporaryFile tmp;
    tmp.open();
    KConfig conf(tmp.fileName());
    KConfigGroup displayAttribute = conf.group(QStringLiteral("Display"));

    KConfigGroup expireAttribute = conf.group(QStringLiteral("Expire"));

    QMapIterator<QString, AttributeInfo> i(lstAttributeInfo);
    while (i.hasNext()) {
        i.next();
        auto attr = i.value();
        if (!attr.displayAttribute.isEmpty()) {
            displayAttribute.writeEntry(i.key(), attr.displayAttribute);
        }
        if (!attr.expireAttribute.isEmpty()) {
            expireAttribute.writeEntry(i.key(), attr.expireAttribute);
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
