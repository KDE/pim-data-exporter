/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SelectionTypeDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectionTypeDialogTest(QObject *parent = nullptr);
    ~SelectionTypeDialogTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHideButtons();

    void shouldSelectAllItems();
};

