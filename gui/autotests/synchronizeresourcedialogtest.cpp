/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synchronizeresourcedialogtest.h"
#include "../dialog/synchronizeresourcedialog.h"
#include <KListWidgetSearchLine>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QStandardPaths>
#include <QTest>

#include <QLabel>
#include <QPushButton>

SynchronizeResourceDialogTest::SynchronizeResourceDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SynchronizeResourceDialogTest::~SynchronizeResourceDialogTest()
{
}

void SynchronizeResourceDialogTest::shouldHaveDefaultValue()
{
    SynchronizeResourceDialog dlg;
    auto buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto listWidget = dlg.findChild<QListWidget *>(QStringLiteral("listresourcewidget"));
    QVERIFY(listWidget);

    auto searchLine = dlg.findChild<KListWidgetSearchLine *>(QStringLiteral("listwidgetsearchline"));
    QVERIFY(searchLine);

    auto label = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(label->wordWrap());

    auto selectAll = dlg.findChild<QPushButton *>(QStringLiteral("selectall_button"));
    QVERIFY(selectAll);

    auto unselectAll = dlg.findChild<QPushButton *>(QStringLiteral("unselectall_button"));
    QVERIFY(unselectAll);

    QVERIFY(dlg.resources().isEmpty());
}

void SynchronizeResourceDialogTest::shouldNotEmptyList()
{
    SynchronizeResourceDialog dlg;
    auto listWidget = dlg.findChild<QListWidget *>(QStringLiteral("listresourcewidget"));
    QHash<QString, QString> lst;
    lst.insert(QStringLiteral("foo"), QStringLiteral("foo"));
    lst.insert(QStringLiteral("faa"), QStringLiteral("faa"));
    dlg.setResources(lst);
    QCOMPARE(dlg.resources().count(), 0);
    QCOMPARE(listWidget->count(), lst.count());
}

QTEST_MAIN(SynchronizeResourceDialogTest)
