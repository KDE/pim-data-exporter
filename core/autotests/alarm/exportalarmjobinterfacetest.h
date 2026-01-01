/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAlarmJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportAlarm();
    void exportAlarm_data();
};
