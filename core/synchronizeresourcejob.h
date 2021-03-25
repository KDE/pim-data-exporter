/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QStringList>
#include "pimdataexporter_export.h"
class KJob;
class PIMDATAEXPORTER_EXPORT SynchronizeResourceJob : public QObject
{
    Q_OBJECT
public:
    explicit SynchronizeResourceJob(QObject *parent = nullptr);
    ~SynchronizeResourceJob();

    void start();
    void setListResources(const QStringList &resources);
    void setSynchronizeOnlyCollection(bool onlyCollection);

Q_SIGNALS:
    void synchronizationFinished();
    void synchronizationInstanceDone(const QString &name, const QString &identifier);
    void synchronizationInstanceFailed(const QString &);

private:
    void slotSynchronizationFinished(KJob *);
    void slotNextSync();

    QStringList mListResources;
    int mIndex = 0;
    bool mOnlyCollection = true;
};

