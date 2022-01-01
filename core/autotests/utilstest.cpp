/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "utilstest.h"
#include "../utils.h"
#include <QTest>
QTEST_GUILESS_MAIN(UtilsTest)
UtilsTest::UtilsTest(QObject *parent)
    : QObject(parent)
{
}

void UtilsTest::shouldTestValue()
{
    QCOMPARE(Utils::configsPath(), QStringLiteral("configs/"));
    QCOMPARE(Utils::transportsPath(), QStringLiteral("transports/"));
    QCOMPARE(Utils::resourcesPath(), QStringLiteral("resources/"));
    QCOMPARE(Utils::identitiesPath(), QStringLiteral("identities/"));
    QCOMPARE(Utils::mailsPath(), QStringLiteral("mails/"));
    QCOMPARE(Utils::configsPath(), QStringLiteral("configs/"));
    QCOMPARE(Utils::akonadiPath(), QStringLiteral("akonadi/"));
    QCOMPARE(Utils::dataPath(), QStringLiteral("data/"));
    QCOMPARE(Utils::calendarPath(), QStringLiteral("calendar/"));
    QCOMPARE(Utils::addressbookPath(), QStringLiteral("addressbook/"));
    QCOMPARE(Utils::alarmPath(), QStringLiteral("alarm/"));
    QCOMPARE(Utils::dataPath(), QStringLiteral("data/"));
    QCOMPARE(Utils::notePath(), QStringLiteral("note/"));
}
