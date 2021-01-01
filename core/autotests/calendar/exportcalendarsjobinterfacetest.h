/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTCALENDARSJOBINTERFACETEST_H
#define EXPORTCALENDARSJOBINTERFACETEST_H

#include <QObject>

class ExportCalendarsJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportCalendarsJobInterfaceTest(QObject *parent = nullptr);
    ~ExportCalendarsJobInterfaceTest() = default;
private Q_SLOTS:
    void exportCalendar_data();
    void exportCalendar();
};

#endif // EXPORTCALENDARSJOBINTERFACETEST_H
