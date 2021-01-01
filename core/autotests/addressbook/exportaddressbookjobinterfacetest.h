/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTADDRESSBOOKJOBINTERFACETEST_H
#define EXPORTADDRESSBOOKJOBINTERFACETEST_H

#include <QObject>

class ExportAddressbookJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAddressbookJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAddressbookJobInterfaceTest() = default;
private Q_SLOTS:
    void exportAddressBook();
    void exportAddressBook_data();
};

#endif // EXPORTADDRESSBOOKJOBINTERFACETEST_H
