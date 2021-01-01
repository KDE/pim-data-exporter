/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOGINFILETEST_H
#define LOGINFILETEST_H

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

#endif // LOGINFILETEST_H
