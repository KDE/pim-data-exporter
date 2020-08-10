/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTALARMJOBINTERFACETEST_H
#define IMPORTALARMJOBINTERFACETEST_H

#include <QObject>

class ImportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAlarmJobInterfaceTest() = default;
private Q_SLOTS:
    void importAlarmConfig();
    void importAlarmConfig_data();
};

#endif // IMPORTALARMJOBINTERFACETEST_H
