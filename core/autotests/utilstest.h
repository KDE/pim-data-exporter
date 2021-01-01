/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef UTILSTEST_H
#define UTILSTEST_H

#include <QObject>

class UtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit UtilsTest(QObject *parent = nullptr);
    ~UtilsTest() = default;
private Q_SLOTS:
    void shouldTestValue();
};

#endif // UTILSTEST_H
