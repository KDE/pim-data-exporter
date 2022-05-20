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
#include <KConfigGroup>
#include <KZip>
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

    auto job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);
    job->fetchScope().fetchAttribute<MailCommon::ExpireCollectionAttribute>();
    job->fetchScope().fetchAttribute<Akonadi::EntityDisplayAttribute>();
    job->fetchScope().setContentMimeTypes({QStringLiteral("message/rfc822")});
    connect(job, &Akonadi::CollectionFetchJob::result, this, &ExportMailFolderAttributeJob::slotFetchFinished);
}

void ExportMailFolderAttributeJob::setArchive(KZip *zip)
{
    mArchive = zip;
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
    QTemporaryFile tmp;
    tmp.open();
    KConfig conf(tmp.fileName());
    KConfigGroup displayAttribute = conf.group(QStringLiteral("Display"));

    KConfigGroup expireAttribute = conf.group(QStringLiteral("Expire"));
    for (const auto &col : cols) {
        const auto *attr = col.attribute<MailCommon::ExpireCollectionAttribute>();
        if (attr) {
            qDebug() << " col.id : " << col.id() << " serialize" << attr->serialized();
            displayAttribute.writeEntry(QString::number(col.id()), attr->serialized());
        }
        const auto *attrDisplay = col.attribute<Akonadi::EntityDisplayAttribute>();
        if (attrDisplay) {
            qDebug() << " col.id : " << col.id() << " serialize" << attrDisplay->serialized();
            expireAttribute.writeEntry(QString::number(col.id()), attrDisplay->serialized());
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
