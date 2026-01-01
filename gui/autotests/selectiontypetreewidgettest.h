/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
