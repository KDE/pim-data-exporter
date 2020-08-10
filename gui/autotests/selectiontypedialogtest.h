/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTIONTYPEDIALOGTEST_H
#define SELECTIONTYPEDIALOGTEST_H

#include <QObject>

class SelectionTypeDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectionTypeDialogTest(QObject *parent = nullptr);
    ~SelectionTypeDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHideButtons();

    void shouldSelectAllItems();
};

#endif // SELECTIONTYPEDIALOGTEST_H
