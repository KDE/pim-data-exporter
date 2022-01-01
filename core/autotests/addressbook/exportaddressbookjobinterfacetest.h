/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportAddressbookJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAddressbookJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAddressbookJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportAddressBook();
    void exportAddressBook_data();
};
