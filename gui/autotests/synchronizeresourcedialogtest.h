/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SYNCHRONIZERESOURCEDIALOGTEST_H
#define SYNCHRONIZERESOURCEDIALOGTEST_H

#include <QObject>

class SynchronizeResourceDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SynchronizeResourceDialogTest(QObject *parent = nullptr);
    ~SynchronizeResourceDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldNotEmptyList();
};

#endif // SYNCHRONIZERESOURCEDIALOGTEST_H
