/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <MailCommon/ExpireCollectionAttribute>

ExportMailFolderAttributeJob::ExportMailFolderAttributeJob(QObject *parent)
    : QObject{parent}
{
}

ExportMailFolderAttributeJob::~ExportMailFolderAttributeJob()
{
}

bool ExportMailFolderAttributeJob::canStart() const
{
    return true;
}

void ExportMailFolderAttributeJob::start()
{
    if (!canStart()) {
        Q_EMIT finished();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Impossible to start job";
        deleteLater();
        return;
    }

    auto job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);
    job->fetchScope().fetchAttribute<MailCommon::ExpireCollectionAttribute>();
    connect(job, &Akonadi::CollectionFetchJob::result, this, &ExportMailFolderAttributeJob::slotFetchFinished);
}

void ExportMailFolderAttributeJob::slotFetchFinished(KJob *job)
{
    if (job->error()) {
        Q_EMIT finished();
        deleteLater();

        return;
    }

    auto list = static_cast<Akonadi::CollectionFetchJob *>(job);
    const Akonadi::Collection::List cols = list->collections();
    if (cols.isEmpty()) {
        Q_EMIT finished();
        deleteLater();
        return;
    }
    Q_EMIT finished();
    deleteLater();
}
