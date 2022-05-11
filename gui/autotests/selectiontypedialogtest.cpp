/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypedialogtest.h"
#include "../dialog/selectiontypedialog.h"
#include <QStandardPaths>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>
#include <qtestmouse.h>

#include <gui/widgets/selectiontypetreewidget.h>

SelectionTypeDialogTest::SelectionTypeDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectionTypeDialogTest::~SelectionTypeDialogTest()
{
}

void SelectionTypeDialogTest::shouldHaveDefaultValue()
{
    SelectionTypeDialog dlg(false);
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    auto buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto mSelectionTreeWidget = dlg.findChild<SelectionTypeTreeWidget *>(QStringLiteral("mSelectionTreeWidget"));
    QVERIFY(mSelectionTreeWidget);

    auto mUseTemplateByDefault = dlg.findChild<QCheckBox *>(QStringLiteral("mUseTemplateByDefault"));
    QVERIFY(mUseTemplateByDefault);
    QVERIFY(!mUseTemplateByDefault->isChecked());

    auto selectAll = dlg.findChild<QPushButton *>(QStringLiteral("selectAll"));
    QVERIFY(selectAll);
    QVERIFY(selectAll->isEnabled());

    auto unselectAll = dlg.findChild<QPushButton *>(QStringLiteral("unselectAll"));
    QVERIFY(unselectAll);
    QVERIFY(unselectAll->isEnabled());

    auto mSaveTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mSaveTemplate"));
    QVERIFY(mSaveTemplate);
    QVERIFY(mSaveTemplate->isEnabled());

    auto mLoadTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mLoadTemplate"));
    QVERIFY(mLoadTemplate);
    QVERIFY(mLoadTemplate->isEnabled());
}

void SelectionTypeDialogTest::shouldHideButtons()
{
    SelectionTypeDialog dlg(false);
    dlg.removeNotSelectedItems();
    auto selectAll = dlg.findChild<QPushButton *>(QStringLiteral("selectAll"));
    QVERIFY(!selectAll->isHidden());

    auto unselectAll = dlg.findChild<QPushButton *>(QStringLiteral("unselectAll"));
    QVERIFY(!unselectAll->isHidden());

    auto mSaveTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mSaveTemplate"));
    QVERIFY(mSaveTemplate->isHidden());

    auto mLoadTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mLoadTemplate"));
    QVERIFY(mLoadTemplate->isHidden());

    auto mUseTemplateByDefault = dlg.findChild<QCheckBox *>(QStringLiteral("mUseTemplateByDefault"));
    QVERIFY(mUseTemplateByDefault->isHidden());
}

void checkState(SelectionTypeTreeWidget *mSelectionTreeWidget, bool checked)
{
    for (int i = 0; i < mSelectionTreeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = mSelectionTreeWidget->topLevelItem(i);
        bool isChecked = item->checkState(0);
        QCOMPARE(isChecked, checked);
        for (int j = 0; j < item->childCount(); j++) {
            bool childIsChecked = item->child(j)->checkState(0);
            QCOMPARE(childIsChecked, checked);
        }
    }
}

void SelectionTypeDialogTest::shouldSelectAllItems()
{
    SelectionTypeDialog dlg(false);
    auto mSelectionTreeWidget = dlg.findChild<SelectionTypeTreeWidget *>(QStringLiteral("mSelectionTreeWidget"));
    auto selectAll = dlg.findChild<QPushButton *>(QStringLiteral("selectAll"));
    QTest::mouseClick(selectAll, Qt::LeftButton);

    checkState(mSelectionTreeWidget, true);

    auto unselectAll = dlg.findChild<QPushButton *>(QStringLiteral("unselectAll"));
    QTest::mouseClick(unselectAll, Qt::LeftButton);
    checkState(mSelectionTreeWidget, false);
}

QTEST_MAIN(SelectionTypeDialogTest)
