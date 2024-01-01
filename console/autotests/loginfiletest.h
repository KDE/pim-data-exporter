/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LogInFileTest : public QObject
{
    Q_OBJECT
public:
    explicit LogInFileTest(QObject *parent = nullptr);
    ~LogInFileTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
