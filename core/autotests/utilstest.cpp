/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "utilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "../utils.h"
#include <QTest>
QTEST_GUILESS_MAIN(UtilsTest)
UtilsTest::UtilsTest(QObject *parent)
    : QObject(parent)
{
}

void UtilsTest::shouldTestValue()
{
    QCOMPARE(Utils::configsPath(), u"configs/"_s);
    QCOMPARE(Utils::transportsPath(), u"transports/"_s);
    QCOMPARE(Utils::resourcesPath(), u"resources/"_s);
    QCOMPARE(Utils::identitiesPath(), u"identities/"_s);
    QCOMPARE(Utils::mailsPath(), u"mails/"_s);
    QCOMPARE(Utils::configsPath(), u"configs/"_s);
    QCOMPARE(Utils::akonadiPath(), u"akonadi/"_s);
    QCOMPARE(Utils::dataPath(), u"data/"_s);
    QCOMPARE(Utils::calendarPath(), u"calendar/"_s);
    QCOMPARE(Utils::addressbookPath(), u"addressbook/"_s);
    QCOMPARE(Utils::alarmPath(), u"alarm/"_s);
    QCOMPARE(Utils::dataPath(), u"data/"_s);
    QCOMPARE(Utils::notePath(), u"note/"_s);
}

#include "moc_utilstest.cpp"
