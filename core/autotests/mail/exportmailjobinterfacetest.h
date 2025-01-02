/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportMailJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportMailJobInterfaceTest(QObject *parent = nullptr);
    ~ExportMailJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportMail();
    void exportMail_data();
};
