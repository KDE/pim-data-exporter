/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mail/importmailfolderattributejob.h"

class ImportMailFolderAttributeJobTestImpl : public ImportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobTestImpl(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJobTestImpl() override;

protected:
    void applyAttributes() override final;
};
