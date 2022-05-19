/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypetreewidgettest.h"
#include "../widgets/selectiontypetreewidget.h"
#include <QFile>
#include <QStandardPaths>
#include <QTest>
//#define SHOW_WIDGET
SelectionTypeTreeWidgetTest::SelectionTypeTreeWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectionTypeTreeWidgetTest::~SelectionTypeTreeWidgetTest() = default;

void SelectionTypeTreeWidgetTest::shouldHaveDefaultSelectedWidget()
{
    SelectionTypeTreeWidget w(true);
    QVERIFY(w.topLevelItemCount() != 0);
}

void SelectionTypeTreeWidgetTest::shouldLoadTemplate_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("topLevelItems");
    QTest::newRow("selectedtypemodel1.xml") << QStringLiteral("selectedtypemodel1.xml") << 3; // because blogilo is not supported anymore
    QTest::newRow("selectedtypemodel2.xml") << QStringLiteral("selectedtypemodel2.xml") << 1;
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

void SelectionTypeTreeWidgetTest::shouldLoadTemplate()
{
    QFETCH(QString, filename);
    QFETCH(int, topLevelItems);

    QString fileNameFullPath = QLatin1String(PIMDATAEXPORTER_DATA_DIR) + QLatin1String("/") + filename;
    QFile f(fileNameFullPath);
    QVERIFY(f.exists());
    SelectionTypeTreeWidget w(true);
    qDebug() << " filename" << fileNameFullPath;
    w.loadTemplate(fileNameFullPath);
    w.removeNotSelectedItems();
#ifdef SHOW_WIDGET
    w.show();
    QTest::qWait(5000);
#endif
    checkState(&w, true);
    QCOMPARE(w.topLevelItemCount(), topLevelItems);
}

QTEST_MAIN(SelectionTypeTreeWidgetTest)
