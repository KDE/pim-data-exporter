/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejobtest.h"
#include "mail/exportmailfolderattributejob.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(ExportMailFolderAttributeJobTest)
ExportMailFolderAttributeJobTest::ExportMailFolderAttributeJobTest(QObject *parent)
    : QObject{parent}
{
}

void ExportMailFolderAttributeJobTest::shouldHaveDefaultValues()
{
    ExportMailFolderAttributeJob job;
    QVERIFY(!job.canStart());
}

void ExportMailFolderAttributeJobTest::shouldEmitSignal()
{
    ExportMailFolderAttributeJob job;
    QSignalSpy spyFinished(&job, &ExportMailFolderAttributeJob::failed);
    job.start();
    QCOMPARE(spyFinished.count(), 1);
}
