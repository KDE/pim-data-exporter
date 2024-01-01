/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAlarmJobInterfaceTest() override = default;
private Q_SLOTS:
    void importAlarmConfig();
    void importAlarmConfig_data();
};
