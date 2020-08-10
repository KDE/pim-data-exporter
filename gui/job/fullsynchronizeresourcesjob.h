/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FULLSYNCHRONIZERESOURCESJOB_H
#define FULLSYNCHRONIZERESOURCESJOB_H

#include <QObject>
#include <QStringList>

class QWidget;
class QProgressDialog;
class FullSynchronizeResourcesJob : public QObject
{
    Q_OBJECT
public:
    explicit FullSynchronizeResourcesJob(QObject *parent = nullptr);
    ~FullSynchronizeResourcesJob();

    void setResources(const QStringList &lst);

    void setWindowParent(QWidget *parent);

    void start();
Q_SIGNALS:
    void synchronizeFinished();
    void synchronizeInstanceDone(const QString &instance);
    void synchronizeInstanceFailed(const QString &instance);

private:
    void slotSynchronizeInstanceFailed(const QString &identifier);
    void slotSynchronizeInstanceDone(const QString &identifier);
    void slotSynchronizeFinished();

    QStringList mResources;
    QWidget *mWindowParent = nullptr;
    QProgressDialog *mProgressDialog = nullptr;
};

#endif // FULLSYNCHRONIZERESOURCESJOB_H
