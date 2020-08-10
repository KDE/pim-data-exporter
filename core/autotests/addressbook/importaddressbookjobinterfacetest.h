/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTADDRESSBOOKJOBINTERFACETEST_H
#define IMPORTADDRESSBOOKJOBINTERFACETEST_H

#include <QObject>
class ImportAddressbookJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAddressbookJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAddressbookJobInterfaceTest() = default;
private Q_SLOTS:
    void importAddressbook();
    void importAddressbook_data();
};

#endif // IMPORTADDRESSBOOKJOBINTERFACETEST_H
