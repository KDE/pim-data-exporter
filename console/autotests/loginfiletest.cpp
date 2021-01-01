/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loginfiletest.h"
#include <QTest>
#include "../loginfile.h"
LogInFileTest::LogInFileTest(QObject *parent)
    : QObject(parent)
{
}

LogInFileTest::~LogInFileTest()
{
}

void LogInFileTest::shouldHaveDefaultValue()
{
    LogInFile logFile;
    QVERIFY(logFile.fileName().isEmpty());
}

QTEST_MAIN(LogInFileTest)
