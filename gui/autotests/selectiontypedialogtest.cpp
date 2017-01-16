/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

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

#include "selectiontypedialogtest.h"
#include "../dialog/selectiontypedialog.h"
#include <QStandardPaths>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>

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
    SelectionTypeDialog dlg;
    QVERIFY(dlg.isModal());
    QVERIFY(!dlg.windowTitle().isEmpty());

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    SelectionTypeTreeWidget *mSelectionTreeWidget = dlg.findChild<SelectionTypeTreeWidget *>(QStringLiteral("mSelectionTreeWidget"));
    QVERIFY(mSelectionTreeWidget);

    QCheckBox *mUseTemplateByDefault = dlg.findChild<QCheckBox*>(QStringLiteral("mUseTemplateByDefault"));
    QVERIFY(mUseTemplateByDefault);
    QVERIFY(!mUseTemplateByDefault->isChecked());

    QPushButton *selectAll = dlg.findChild<QPushButton *>(QStringLiteral("selectAll"));
    QVERIFY(selectAll);
    QVERIFY(selectAll->isEnabled());

    QPushButton *unselectAll = dlg.findChild<QPushButton *>(QStringLiteral("unselectAll"));
    QVERIFY(unselectAll);
    QVERIFY(unselectAll->isEnabled());

    QPushButton *mSaveTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mSaveTemplate"));
    QVERIFY(mSaveTemplate);
    QVERIFY(mSaveTemplate->isEnabled());

    QPushButton *mLoadTemplate = dlg.findChild<QPushButton *>(QStringLiteral("mLoadTemplate"));
    QVERIFY(mLoadTemplate);
    QVERIFY(mLoadTemplate->isEnabled());
}

QTEST_MAIN(SelectionTypeDialogTest)
