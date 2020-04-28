/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "utilstest.h"
#include "../utils.h"
#include <QTest>
QTEST_GUILESS_MAIN(UtilsTest)
UtilsTest::UtilsTest(QObject *parent) : QObject(parent)
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

