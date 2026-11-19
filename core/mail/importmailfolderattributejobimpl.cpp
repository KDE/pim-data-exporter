/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejobimpl.h"
#include "pimdataexportcore_debug.h"
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/CollectionModifyJob>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/FavoriteCollectionAttribute>
#include <MailCommon/ExpireCollectionAttribute>

ImportMailFolderAttributeJobImpl::ImportMailFolderAttributeJobImpl(QObject *parent)
    : ImportMailFolderAttributeJob{parent}
{
}

ImportMailFolderAttributeJobImpl::~ImportMailFolderAttributeJobImpl() = default;

void ImportMailFolderAttributeJobImpl::applyAttributes(const QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> &map)
{
    mAttributeMap = map;
    mCurrentAttribute = mAttributeMap.cend();
    mNextAttribute = mAttributeMap.cbegin();
    nextAttribute();
}

void ImportMailFolderAttributeJobImpl::nextAttribute()
{
    if (mNextAttribute != mAttributeMap.cend()) {
        mCurrentAttribute = mNextAttribute;
        ++mNextAttribute;
        qDebug() << " restoring folder attribute " << mCurrentAttribute.key();
        if (mCurrentAttribute.key() == -1) {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << " It's a bug !  restoring folder attribute " << mCurrentAttribute.key();
            nextAttribute();
            return;
        }
        auto fetch = new Akonadi::CollectionFetchJob(Akonadi::Collection(mCurrentAttribute.key()), Akonadi::CollectionFetchJob::Base, this);
        fetch->fetchScope().fetchAttribute<Akonadi::EntityDisplayAttribute>();
        fetch->fetchScope().fetchAttribute<MailCommon::ExpireCollectionAttribute>();
        connect(fetch, &Akonadi::CollectionFetchJob::result, this, &ImportMailFolderAttributeJobImpl::collectionFetchResult);
        connect(fetch, &Akonadi::CollectionFetchJob::collectionsReceived, this, [this](const Akonadi::Collection::List &cols) {
            if (cols.count() != 1) {
                nextAttribute();
                return;
            }
            Akonadi::Collection col = cols.first();
            if (!mCurrentAttribute.value().expireAttribute.isEmpty()) {
                MailCommon::ExpireCollectionAttribute *expireAttribute =
                    col.attribute<MailCommon::ExpireCollectionAttribute>(Akonadi::Collection::AddIfMissing);
                expireAttribute->deserialize(mCurrentAttribute.value().expireAttribute);
            }
            if (!mCurrentAttribute.value().displayAttribute.isEmpty()) {
                Akonadi::EntityDisplayAttribute *entityAttribute = col.attribute<Akonadi::EntityDisplayAttribute>(Akonadi::Collection::AddIfMissing);
                entityAttribute->deserialize(mCurrentAttribute.value().displayAttribute);
            }
            if (!mCurrentAttribute.value().favoriteAttribute.isEmpty()) {
                Akonadi::FavoriteCollectionAttribute *favoriteAttribute =
                    col.attribute<Akonadi::FavoriteCollectionAttribute>(Akonadi::Collection::AddIfMissing);
                favoriteAttribute->deserialize(mCurrentAttribute.value().favoriteAttribute);
            }

            qDebug() << " modify folder attribute " << mCurrentAttribute.key();
            auto job = new Akonadi::CollectionModifyJob(col, this);
            connect(job, &Akonadi::CollectionModifyJob::result, this, &ImportMailFolderAttributeJobImpl::slotCollectionModifyDone);
        });
    } else {
        // Call it when all is finished!
        qDebug() << " restoring folder attribute finished";
        restoreFileFolderAttribute();
    }
}

void ImportMailFolderAttributeJobImpl::collectionFetchResult(KJob *job)
{
    if (job->error()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Error when we fetch collection: " << job->errorString();
        nextAttribute();
    }
}

void ImportMailFolderAttributeJobImpl::slotCollectionModifyDone(KJob *job)
{
    if (job->error()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Error when we modified collection: " << job->errorString();
    }
    nextAttribute();
}

#include "moc_importmailfolderattributejobimpl.cpp"
