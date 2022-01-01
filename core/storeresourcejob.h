/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class KZip;
class StoreResourceJob : public QObject
{
    Q_OBJECT
public:
    explicit StoreResourceJob(QObject *parent = nullptr);
    ~StoreResourceJob() override;

    void start();

    void setArchivePath(const QString &archivePath);

    void setIdentifier(const QString &identifier);

    void setZip(KZip *zip);
Q_SIGNALS:
    void error(const QString &);
    void info(const QString &);

private:
    QString mArchivePath;
    QString mIdentifier;
    KZip *mZip = nullptr;
};
