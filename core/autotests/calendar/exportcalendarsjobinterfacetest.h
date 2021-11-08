/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportCalendarsJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportCalendarsJobInterfaceTest(QObject *parent = nullptr);
    ~ExportCalendarsJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportCalendar_data();
    void exportCalendar();
};
