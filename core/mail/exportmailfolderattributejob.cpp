/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"
#include "utils.h"
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/EntityDisplayAttribute>
#include <MailCommon/ExpireCollectionAttribute>
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
    return (mZip != nullptr);
}

void ExportMailFolderAttributeJob::start()
{
    if (!canStart()) {
        Q_EMIT failed();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Impossible to start job";
        deleteLater();
        return;
    }

    auto job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);
    job->fetchScope().fetchAttribute<MailCommon::ExpireCollectionAttribute>();
    job->fetchScope().fetchAttribute<Akonadi::EntityDisplayAttribute>();
    job->fetchScope().setContentMimeTypes({QStringLiteral("message/rfc822")});
    connect(job, &Akonadi::CollectionFetchJob::result, this, &ExportMailFolderAttributeJob::slotFetchFinished);
}

void ExportMailFolderAttributeJob::slotFetchFinished(KJob *job)
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
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "It seems wierd that there is not collection.";
        deleteLater();
        return;
    }
    // TODO store data
    for (const auto &col : cols) {
        const auto *attr = col.attribute<MailCommon::ExpireCollectionAttribute>();
        if (attr) {
            qDebug() << " col.id : " << col.id() << " serialize" << attr->serialized();
        }
        const auto *attrDisplay = col.attribute<Akonadi::EntityDisplayAttribute>();
        if (attrDisplay) {
            qDebug() << " col.id : " << col.id() << " serialize" << attrDisplay->serialized();
        }
    }
#if 0
    QTemporaryFile tmp;
    tmp.open();
    const QUrl url = QUrl::fromLocalFile(tmp.fileName());
    MailCommon::FilterImporterExporter exportFilters;
    exportFilters.exportFilters(lstFilter, url, true);
    tmp.close();
    const bool fileAdded = mZip->addLocalFile(tmp.fileName(), Utils::configsPath() + QStringLiteral("mailfolderattributes"));
#endif

    Q_EMIT successed();
    deleteLater();
}
