/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTMAILJOBINTERFACETEST_H
#define EXPORTMAILJOBINTERFACETEST_H

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

#endif // EXPORTMAILJOBINTERFACETEST_H
