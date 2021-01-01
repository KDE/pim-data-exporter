/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BACKUPRESOURCEFILEJOBBASE_H
#define BACKUPRESOURCEFILEJOBBASE_H

#include <QObject>
#include <KZip>

#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT BackupResourceFileJobBase : public QObject
{
    Q_OBJECT
public:
    explicit BackupResourceFileJobBase(QObject *parent = nullptr);
    ~BackupResourceFileJobBase();

    void start();

    Q_REQUIRED_RESULT bool canStart() const;

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT KZip *zip() const;
    void setZip(KZip *zip);

    Q_REQUIRED_RESULT QString defaultPath() const;
    void setDefaultPath(const QString &defaultPath);

    virtual Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const = 0;
Q_SIGNALS:
    void error(const QString &);
    void info(const QString &);

private:
    QString mDefaultPath;
    QString mIdentifier;
    KZip *mZip = nullptr;
};

#endif // BACKUPRESOURCEFILEJOBBASE_H
