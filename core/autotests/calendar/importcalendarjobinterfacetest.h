/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTCALENDARJOBINTERFACETEST_H
#define IMPORTCALENDARJOBINTERFACETEST_H

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

#endif // IMPORTCALENDARJOBINTERFACETEST_H
