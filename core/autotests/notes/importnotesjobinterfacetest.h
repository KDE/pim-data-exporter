/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportNotesJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterfaceTest(QObject *parent = nullptr);
    ~ImportNotesJobInterfaceTest() override = default;
private Q_SLOTS:
    void importNote();
    void importNote_data();
};

