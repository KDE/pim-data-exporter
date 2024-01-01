/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mail/exportmailfolderattributejob.h"

class ExportMailFolderAttributeJobTestImpl : public ExportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ExportMailFolderAttributeJobTestImpl(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJobTestImpl() override = default;

protected:
    void fetchAttributes() override final;
};
