/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STORERESOURCEJOB_H
#define STORERESOURCEJOB_H

#include <QObject>
class KZip;
class StoreResourceJob : public QObject
{
    Q_OBJECT
public:
    explicit StoreResourceJob(QObject *parent = nullptr);
    ~StoreResourceJob();

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

#endif // STORERESOURCEJOB_H
