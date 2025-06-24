/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "../dialog/selectiontypedialog.h"
#include <QStandardPaths>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>
#include <qtestmouse.h>

#include "gui/widgets/selectiontypetreewidget.h"

SelectionTypeDialogTest::SelectionTypeDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectionTypeDialogTest::~SelectionTypeDialogTest() = default;

void SelectionTypeDialogTest::shouldHaveDefaultValue()
{
    SelectionTypeDialog dlg(false);
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    auto buttonBox = dlg.findChild<QDialogButtonBox *>(u"buttonbox"_s);
    QVERIFY(buttonBox);

    auto mSelectionTreeWidget = dlg.findChild<SelectionTypeTreeWidget *>(u"mSelectionTreeWidget"_s);
    QVERIFY(mSelectionTreeWidget);

    auto mUseTemplateByDefault = dlg.findChild<QCheckBox *>(u"mUseTemplateByDefault"_s);
    QVERIFY(mUseTemplateByDefault);
    QVERIFY(!mUseTemplateByDefault->isChecked());

    auto selectAll = dlg.findChild<QPushButton *>(u"selectAll"_s);
    QVERIFY(selectAll);
    QVERIFY(selectAll->isEnabled());

    auto unselectAll = dlg.findChild<QPushButton *>(u"unselectAll"_s);
    QVERIFY(unselectAll);
    QVERIFY(unselectAll->isEnabled());

    auto mSaveTemplate = dlg.findChild<QPushButton *>(u"mSaveTemplate"_s);
    QVERIFY(mSaveTemplate);
    QVERIFY(mSaveTemplate->isEnabled());

    auto mLoadTemplate = dlg.findChild<QPushButton *>(u"mLoadTemplate"_s);
    QVERIFY(mLoadTemplate);
    QVERIFY(mLoadTemplate->isEnabled());
}

void SelectionTypeDialogTest::shouldHideButtons()
{
    SelectionTypeDialog dlg(false);
    dlg.removeNotSelectedItems();
    auto selectAll = dlg.findChild<QPushButton *>(u"selectAll"_s);
    QVERIFY(!selectAll->isHidden());

    auto unselectAll = dlg.findChild<QPushButton *>(u"unselectAll"_s);
    QVERIFY(!unselectAll->isHidden());

    auto mSaveTemplate = dlg.findChild<QPushButton *>(u"mSaveTemplate"_s);
    QVERIFY(mSaveTemplate->isHidden());

    auto mLoadTemplate = dlg.findChild<QPushButton *>(u"mLoadTemplate"_s);
    QVERIFY(mLoadTemplate->isHidden());

    auto mUseTemplateByDefault = dlg.findChild<QCheckBox *>(u"mUseTemplateByDefault"_s);
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
    auto mSelectionTreeWidget = dlg.findChild<SelectionTypeTreeWidget *>(u"mSelectionTreeWidget"_s);
    auto selectAll = dlg.findChild<QPushButton *>(u"selectAll"_s);
    QTest::mouseClick(selectAll, Qt::LeftButton);

    checkState(mSelectionTreeWidget, true);

    auto unselectAll = dlg.findChild<QPushButton *>(u"unselectAll"_s);
    QTest::mouseClick(unselectAll, Qt::LeftButton);
    checkState(mSelectionTreeWidget, false);
}

QTEST_MAIN(SelectionTypeDialogTest)

#include "moc_selectiontypedialogtest.cpp"
