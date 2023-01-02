/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class ImportAddressbookJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAddressbookJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAddressbookJobInterfaceTest() override = default;
private Q_SLOTS:
    void importAddressbook();
    void importAddressbook_data();
};
