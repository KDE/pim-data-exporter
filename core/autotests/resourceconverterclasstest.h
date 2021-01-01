/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RESOURCECONVERTERCLASSTEST_H
#define RESOURCECONVERTERCLASSTEST_H

#include <QObject>

class ResourceConverterClassTest : public QObject
{
    Q_OBJECT
public:
    explicit ResourceConverterClassTest(QObject *parent = nullptr);
    ~ResourceConverterClassTest() = default;
private Q_SLOTS:
    void shouldGetAgentFileName();
};

#endif // RESOURCECONVERTERCLASSTEST_H
