/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportAkregatorJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAkregatorJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAkregatorJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportAkregator_data();
    void exportAkregator();
};
