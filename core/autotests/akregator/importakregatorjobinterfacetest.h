/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTAKREGATORJOBINTERFACETEST_H
#define IMPORTAKREGATORJOBINTERFACETEST_H

#include <QObject>

class ImportAkregatorJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAkregatorJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAkregatorJobInterfaceTest() = default;
//private Q_SLOTS: //TODO reactivate
    void importAkegator_data();
    void importAkegator();
};

#endif // IMPORTAKREGATORJOBINTERFACETEST_H
