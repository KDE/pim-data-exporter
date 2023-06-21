/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testbackupresourcefilejob.h"
#include "resourceconvertertest.h"
TestBackupResourceFileJob::TestBackupResourceFileJob(QObject *parent)
    : BackupResourceFileJobBase(parent)
{
}

TestBackupResourceFileJob::~TestBackupResourceFileJob() = default;

QString TestBackupResourceFileJob::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mTestPath);
    const QString url = converter.resourcePath(identifier);
    return url;
}

void TestBackupResourceFileJob::setTestPath(const QString &str)
{
    mTestPath = str;
}

#include "moc_testbackupresourcefilejob.cpp"
