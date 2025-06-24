/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejobimpl.h"
using namespace Qt::Literals::StringLiterals;

#include "exportmailjobinterface.h"
#include "pimdataexportcore_debug.h"
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/FavoriteCollectionAttribute>
#include <Akonadi/MessageFolderAttribute>
#include <MailCommon/ExpireCollectionAttribute>

ExportMailFolderAttributeJobImpl::ExportMailFolderAttributeJobImpl(QObject *parent)
    : ExportMailFolderAttributeJob{parent}
{
}

ExportMailFolderAttributeJobImpl::~ExportMailFolderAttributeJobImpl() = default;

void ExportMailFolderAttributeJobImpl::fetchAttributes()
{
    auto job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);
    job->fetchScope().fetchAttribute<MailCommon::ExpireCollectionAttribute>();
    job->fetchScope().fetchAttribute<Akonadi::EntityDisplayAttribute>();
    job->fetchScope().fetchAttribute<Akonadi::FavoriteCollectionAttribute>();
    job->fetchScope().fetchAttribute<Akonadi::MessageFolderAttribute>();
    job->fetchScope().setContentMimeTypes({u"message/rfc822"_s});
    connect(job, &Akonadi::CollectionFetchJob::result, this, &ExportMailFolderAttributeJobImpl::slotFetchFinished);
}

void ExportMailFolderAttributeJobImpl::slotFetchFinished(KJob *job)
{
    if (job->error()) {
        Q_EMIT failed();
        deleteLater();
        return;
    }

    auto list = static_cast<Akonadi::CollectionFetchJob *>(job);
    const Akonadi::Collection::List cols = list->collections();
    if (cols.isEmpty()) {
        Q_EMIT successed();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "It seems weird that there is not collection.";
        deleteLater();
        return;
    }
    QMap<QString, ImportExportMailUtil::AttributeInfo> lstAttributeInfo;
    for (const auto &col : cols) {
        ImportExportMailUtil::AttributeInfo info;

        const auto *attr = col.attribute<MailCommon::ExpireCollectionAttribute>();
        if (attr) {
            info.expireAttribute = attr->serialized();
        }
        const auto *attrDisplay = col.attribute<Akonadi::EntityDisplayAttribute>();
        if (attrDisplay) {
            info.expireAttribute = attrDisplay->serialized();
        }
        const auto *attrFavorite = col.attribute<Akonadi::FavoriteCollectionAttribute>();
        if (attrFavorite) {
            info.favoriteAttribute = attrFavorite->serialized();
        }
        const auto *attrFolder = col.attribute<Akonadi::MessageFolderAttribute>();
        if (attrFolder) {
            info.folderAttribute = attrFolder->serialized();
        }
        if (info.isValid()) {
            lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(col.id()), info);
        }
    }
    storeFileFolderAttribute(lstAttributeInfo);
}

#include "moc_exportmailfolderattributejobimpl.cpp"
