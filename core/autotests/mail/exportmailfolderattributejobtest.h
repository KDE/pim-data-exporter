/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "mail/exportmailfolderattributejob.h"

class ExportMailFolderAttributeJobImplTest : public ExportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ExportMailFolderAttributeJobImplTest(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJobImplTest() override = default;

protected:
    void fetchAttributes() override final
    {
    }
};

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
