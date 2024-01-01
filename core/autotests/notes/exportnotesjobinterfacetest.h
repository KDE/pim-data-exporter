/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class ExportNotesJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportNotesJobInterfaceTest(QObject *parent = nullptr);
    ~ExportNotesJobInterfaceTest() override = default;
private Q_SLOTS:
    void exportNote_data();
    void exportNote();
};
