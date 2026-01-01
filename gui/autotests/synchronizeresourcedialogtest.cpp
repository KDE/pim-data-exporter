/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synchronizeresourcedialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

SynchronizeResourceDialogTest::~SynchronizeResourceDialogTest() = default;

void SynchronizeResourceDialogTest::shouldHaveDefaultValue()
{
    SynchronizeResourceDialog dlg;
    auto buttonBox = dlg.findChild<QDialogButtonBox *>(u"buttonbox"_s);
    QVERIFY(buttonBox);

    auto listWidget = dlg.findChild<QListWidget *>(u"listresourcewidget"_s);
    QVERIFY(listWidget);

    auto searchLine = dlg.findChild<KListWidgetSearchLine *>(u"listwidgetsearchline"_s);
    QVERIFY(searchLine);

    auto label = dlg.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(label->wordWrap());

    auto selectAll = dlg.findChild<QPushButton *>(u"selectall_button"_s);
    QVERIFY(selectAll);

    auto unselectAll = dlg.findChild<QPushButton *>(u"unselectall_button"_s);
    QVERIFY(unselectAll);

    QVERIFY(dlg.resources().isEmpty());
}

void SynchronizeResourceDialogTest::shouldNotEmptyList()
{
    SynchronizeResourceDialog dlg;
    auto listWidget = dlg.findChild<QListWidget *>(u"listresourcewidget"_s);
    QHash<QString, QString> lst;
    lst.insert(u"foo"_s, u"foo"_s);
    lst.insert(u"faa"_s, u"faa"_s);
    dlg.setResources(lst);
    QCOMPARE(dlg.resources().count(), 0);
    QCOMPARE(listWidget->count(), lst.count());
}

QTEST_MAIN(SynchronizeResourceDialogTest)

#include "moc_synchronizeresourcedialogtest.cpp"
