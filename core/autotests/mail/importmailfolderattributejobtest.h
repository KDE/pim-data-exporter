/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mail/importmailfolderattributejob.h"
#include <QObject>

class ImportMailFolderAttributeJobImplTest : public ImportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobImplTest(QObject *parent = nullptr)
        : ImportMailFolderAttributeJob(parent)
    {
    }
    ~ImportMailFolderAttributeJobImplTest() override = default;

protected:
    void applyAttributes() override
    {
        // Nothing
    }
};

class ImportMailFolderAttributeJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobTest(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJobTest() override = default;

private Q_SLOTS:
    void shouldEmitSignal();
    void shouldHaveDefaultValues();
};
