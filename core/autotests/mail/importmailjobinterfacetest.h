/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportMailJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTest(QObject *parent = nullptr);
    ~ImportMailJobInterfaceTest() override = default;
private Q_SLOTS:
    void importMail();
    void importMail_data();
};
