/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTNOTESJOBINTERFACETEST_H
#define EXPORTNOTESJOBINTERFACETEST_H

#include <QObject>
class ExportNotesJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportNotesJobInterfaceTest(QObject *parent = nullptr);
    ~ExportNotesJobInterfaceTest() = default;
private Q_SLOTS:
    void exportNote_data();
    void exportNote();
};

#endif // EXPORTNOTESJOBINTERFACETEST_H
