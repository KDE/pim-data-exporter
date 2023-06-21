/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loginfiletest.h"
#include "../loginfile.h"
#include <QTest>
LogInFileTest::LogInFileTest(QObject *parent)
    : QObject(parent)
{
}

LogInFileTest::~LogInFileTest() = default;

void LogInFileTest::shouldHaveDefaultValue()
{
    LogInFile logFile;
    QVERIFY(logFile.fileName().isEmpty());
}

QTEST_MAIN(LogInFileTest)

#include "moc_loginfiletest.cpp"
