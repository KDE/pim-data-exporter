/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejobtest.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(ExportMailFolderAttributeJobTest)

ExportMailFolderAttributeJobTest::ExportMailFolderAttributeJobTest(QObject *parent)
    : QObject{parent}
{
}

void ExportMailFolderAttributeJobTest::shouldHaveDefaultValues()
{
    ExportMailFolderAttributeJobImplTest job;
    QVERIFY(!job.canStart());
}

void ExportMailFolderAttributeJobTest::shouldEmitSignal()
{
    // Emit signal when we can't start.
    ExportMailFolderAttributeJobImplTest job;
    QSignalSpy spyFinished(&job, &ExportMailFolderAttributeJobImplTest::failed);
    job.start();
    QCOMPARE(spyFinished.count(), 1);
}

ExportMailFolderAttributeJobImplTest::ExportMailFolderAttributeJobImplTest(QObject *parent)
    : ExportMailFolderAttributeJob(parent)
{
}
