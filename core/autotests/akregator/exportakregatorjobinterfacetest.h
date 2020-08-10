/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTAKREGATORJOBINTERFACETEST_H
#define EXPORTAKREGATORJOBINTERFACETEST_H

#include <QObject>

class ExportAkregatorJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAkregatorJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAkregatorJobInterfaceTest() = default;
private Q_SLOTS:
    void exportAkregator_data();
    void exportAkregator();
};

#endif // EXPORTAKREGATORJOBINTERFACETEST_H
