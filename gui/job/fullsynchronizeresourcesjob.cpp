/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "fullsynchronizeresourcesjob.h"

#include <KLocalizedString>

#include <QProgressDialog>
#include <synchronizeresourcejob.h>

FullSynchronizeResourcesJob::FullSynchronizeResourcesJob(QObject *parent)
    : QObject(parent)
{
}

FullSynchronizeResourcesJob::~FullSynchronizeResourcesJob()
{
}

void FullSynchronizeResourcesJob::setResources(const QStringList &lst)
{
    if (lst.isEmpty()) {
        Q_EMIT synchronizeFinished();
        deleteLater();
    } else {
        mResources = lst;
    }
}

void FullSynchronizeResourcesJob::setWindowParent(QWidget *parent)
{
    mWindowParent = parent;
}

void FullSynchronizeResourcesJob::start()
{
    if (!mProgressDialog) {
        mProgressDialog = new QProgressDialog(QString(), i18n("Cancel"), 0, mResources.count(), mWindowParent);
    }
    mProgressDialog->setWindowTitle(i18nc("@title:window", "Synchronize resources"));
    mProgressDialog->setLabelText(i18n("Synchronize resources... It can take some time."));
    mProgressDialog->setWindowModality(Qt::WindowModal);
    //Disable cancel button.
    mProgressDialog->setCancelButton(nullptr);

    auto *job = new SynchronizeResourceJob(this);
    //Full synch
    job->setSynchronizeOnlyCollection(false);
    job->setListResources(mResources);
    connect(job, &SynchronizeResourceJob::synchronizationFinished, this, &FullSynchronizeResourcesJob::slotSynchronizeFinished);
    connect(job, &SynchronizeResourceJob::synchronizationInstanceDone, this, &FullSynchronizeResourcesJob::slotSynchronizeInstanceDone);
    connect(job, &SynchronizeResourceJob::synchronizationInstanceFailed, this, &FullSynchronizeResourcesJob::slotSynchronizeInstanceFailed);
    job->start();
}

void FullSynchronizeResourcesJob::slotSynchronizeInstanceDone(const QString &identifier)
{
    Q_EMIT synchronizeInstanceDone(identifier);
    mProgressDialog->setValue(mProgressDialog->value() + 1);
}

void FullSynchronizeResourcesJob::slotSynchronizeInstanceFailed(const QString &identifier)
{
    Q_EMIT synchronizeInstanceFailed(identifier);
    mProgressDialog->setValue(mProgressDialog->value() + 1);
}

void FullSynchronizeResourcesJob::slotSynchronizeFinished()
{
    mProgressDialog->setValue(mProgressDialog->value() + 1);
    Q_EMIT synchronizeFinished();
    deleteLater();
}
