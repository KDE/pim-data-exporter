/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QStringList>

class QWidget;
class QProgressDialog;
class FullSynchronizeResourcesJob : public QObject
{
    Q_OBJECT
public:
    explicit FullSynchronizeResourcesJob(QObject *parent = nullptr);
    ~FullSynchronizeResourcesJob() override;

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
