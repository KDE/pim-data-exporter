/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTMAILJOBINTERFACETEST_H
#define IMPORTMAILJOBINTERFACETEST_H

#include <QObject>

class ImportMailJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTest(QObject *parent = nullptr);
    ~ImportMailJobInterfaceTest() = default;
private Q_SLOTS:
    void importMail();
    void importMail_data();
};

#endif // IMPORTMAILJOBINTERFACETEST_H
