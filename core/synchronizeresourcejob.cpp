/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synchronizeresourcejob.h"

#include <AkonadiCore/resourcesynchronizationjob.h>
#include <AkonadiCore/AgentInstance>
#include <AkonadiCore/AgentManager>

#include <QTimer>
#include "pimdataexportcore_debug.h"

SynchronizeResourceJob::SynchronizeResourceJob(QObject *parent)
    : QObject(parent)
{
}

SynchronizeResourceJob::~SynchronizeResourceJob()
{
}

void SynchronizeResourceJob::start()
{
    if (!mListResources.isEmpty()) {
        QTimer::singleShot(0, this, &SynchronizeResourceJob::slotNextSync);
    } else {
        Q_EMIT synchronizationFinished();
    }
}

void SynchronizeResourceJob::slotNextSync()
{
    if (mIndex < mListResources.count()) {
        const Akonadi::AgentInstance resource = Akonadi::AgentManager::self()->instance(mListResources.at(mIndex));
        qCDebug(PIMDATAEXPORTERCORE_LOG) << " resource.name" << resource.name();
        Akonadi::ResourceSynchronizationJob *job = new Akonadi::ResourceSynchronizationJob(resource);
        job->setTimeoutCountLimit(10);
        job->setCollectionTreeOnly(mOnlyCollection);
        connect(job, &Akonadi::ResourceSynchronizationJob::result, this, &SynchronizeResourceJob::slotSynchronizationFinished);
        job->start();
    } else {
        Q_EMIT synchronizationFinished();
    }
}

void SynchronizeResourceJob::slotSynchronizationFinished(KJob *job)
{
    Akonadi::ResourceSynchronizationJob *resourceSync = qobject_cast<Akonadi::ResourceSynchronizationJob *>(job);
    const QString instanceName = resourceSync->resource().name();
    if (job->error()) {
        Q_EMIT synchronizationInstanceFailed(instanceName);
    } else {
        Q_EMIT synchronizationInstanceDone(instanceName, resourceSync->resource().identifier());
    }
    ++mIndex;
    QTimer::singleShot(0, this, &SynchronizeResourceJob::slotNextSync);
}

void SynchronizeResourceJob::setListResources(const QStringList &resources)
{
    mListResources = resources;
}

void SynchronizeResourceJob::setSynchronizeOnlyCollection(bool onlyCollection)
{
    mOnlyCollection = onlyCollection;
}
