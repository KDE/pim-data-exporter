/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTIONTYPETREEWIDGETTEST_H
#define SELECTIONTYPETREEWIDGETTEST_H

#include <QObject>

class SelectionTypeTreeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectionTypeTreeWidgetTest(QObject *parent = nullptr);
    ~SelectionTypeTreeWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultSelectedWidget();
    void shouldLoadTemplate();
    void shouldLoadTemplate_data();
};

#endif // SELECTIONTYPETREEWIDGETTEST_H
