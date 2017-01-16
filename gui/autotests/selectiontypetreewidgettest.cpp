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


#include "selectiontypetreewidgettest.h"
#include "../widgets/selectiontypetreewidget.h"
#include <QFile>
#include <QTest>
#include <QStandardPaths>
//#define SHOW_WIDGET
SelectionTypeTreeWidgetTest::SelectionTypeTreeWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectionTypeTreeWidgetTest::~SelectionTypeTreeWidgetTest()
{

}

void SelectionTypeTreeWidgetTest::shouldHaveDefaultSelectedWidget()
{
    SelectionTypeTreeWidget w;
    QVERIFY(w.topLevelItemCount() != 0);
}

void SelectionTypeTreeWidgetTest::shouldLoadTemplate_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("numberItems");
    QTest::newRow("selectedtypemodel1.xml") << QStringLiteral("selectedtypemodel1.xml") << 7;
}

void SelectionTypeTreeWidgetTest::shouldLoadTemplate()
{
    QFETCH(QString, filename);
    QFETCH(int, numberItems);


    QString fileNameFullPath = QLatin1String(PIMDATAEXPORTER_DATA_DIR) + QLatin1String("/") + filename;
    QFile f(fileNameFullPath);
    QVERIFY(f.exists());
    SelectionTypeTreeWidget w;
    qDebug() << " filename"<<fileNameFullPath;
    w.loadTemplate(fileNameFullPath);
    w.removeNotSelectedItems();
#ifdef SHOW_WIDGET
    w.show();
    QTest::qWait(5000);
#endif
    QCOMPARE(w.topLevelItemCount(), numberItems);
}


QTEST_MAIN(SelectionTypeTreeWidgetTest)
