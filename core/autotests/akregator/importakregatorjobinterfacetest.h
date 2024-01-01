/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportAkregatorJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAkregatorJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAkregatorJobInterfaceTest() override = default;
    // private Q_SLOTS: //TODO reactivate
    void importAkegator_data();
    void importAkegator();
};
