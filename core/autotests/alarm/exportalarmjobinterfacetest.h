/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ExportAlarmJobInterfaceTest_H
#define ExportAlarmJobInterfaceTest_H

#include <QObject>

class ExportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAlarmJobInterfaceTest() = default;
private Q_SLOTS:
    void exportAlarm();
    void exportAlarm_data();
};

#endif // ExportAlarmJobInterfaceTest_H
