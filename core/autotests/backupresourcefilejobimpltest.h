/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BACKUPRESOURCEFILEJOBTEST_H
#define BACKUPRESOURCEFILEJOBTEST_H

#include <QObject>

class BackupResourceFileJobImplTest : public QObject
{
    Q_OBJECT
public:
    explicit BackupResourceFileJobImplTest(QObject *parent = nullptr);
    ~BackupResourceFileJobImplTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // BACKUPRESOURCEFILEJOBTEST_H
