/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <backupresourcefilejobbase.h>

class TestBackupResourceFileJob : public BackupResourceFileJobBase
{
    Q_OBJECT
public:
    explicit TestBackupResourceFileJob(QObject *parent = nullptr);
    ~TestBackupResourceFileJob() override;

    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void setTestPath(const QString &str);
private:
    QString mTestPath;
};

