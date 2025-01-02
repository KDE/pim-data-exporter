/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synchronizeresourcejob.h"

#include <Akonadi/AgentInstance>
#include <Akonadi/AgentManager>
#include <Akonadi/ResourceSynchronizationJob>

#include "pimdataexportcore_debug.h"
#include <QTimer>

SynchronizeResourceJob::SynchronizeResourceJob(QObject *parent)
    : QObject(parent)
{
}

SynchronizeResourceJob::~SynchronizeResourceJob() = default;

void SynchronizeResourceJob::start()
{
    if (mListResources.isEmpty()) {
        Q_EMIT synchronizationFinished();
    } else {
        QTimer::singleShot(0, this, &SynchronizeResourceJob::slotNextSync);
    }
}

void SynchronizeResourceJob::slotNextSync()
{
    if (mIndex < mListResources.count()) {
        const Akonadi::AgentInstance resource = Akonadi::AgentManager::self()->instance(mListResources.at(mIndex));
        qCDebug(PIMDATAEXPORTERCORE_LOG) << " resource.name" << resource.name();
        auto job = new Akonadi::ResourceSynchronizationJob(resource);
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
    auto resourceSync = qobject_cast<Akonadi::ResourceSynchronizationJob *>(job);
    const QString instanceName = resourceSync->resource().name();
    if (job->error()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Failed to synchronize :" << job->errorString();
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

#include "moc_synchronizeresourcejob.cpp"
