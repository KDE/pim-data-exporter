/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejobtest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(ImportMailFolderAttributeJobTest)

ImportMailFolderAttributeJobTest::ImportMailFolderAttributeJobTest(QObject *parent)
    : QObject{parent}
{
}

void ImportMailFolderAttributeJobTest::shouldHaveDefaultValues()
{
    ImportMailFolderAttributeJobImplTest job;
    QVERIFY(!job.canStart());
}

void ImportMailFolderAttributeJobTest::shouldEmitSignal()
{
    // Emit signal when we can't start.
    ImportMailFolderAttributeJobImplTest job;
    QSignalSpy spyFinished(&job, &ImportMailFolderAttributeJobImplTest::failed);
    job.start();
    QCOMPARE(spyFinished.count(), 1);
}

#include "moc_importmailfolderattributejobtest.cpp"
