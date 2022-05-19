/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExportMailFolderAttributeJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportMailFolderAttributeJobTest(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignal();
};
