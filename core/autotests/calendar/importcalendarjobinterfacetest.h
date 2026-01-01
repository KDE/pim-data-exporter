/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportCalendarJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportCalendarJobInterfaceTest(QObject *parent = nullptr);
    ~ImportCalendarJobInterfaceTest() override = default;
private Q_SLOTS:
    void importCalendar();
    void importCalendar_data();
};
