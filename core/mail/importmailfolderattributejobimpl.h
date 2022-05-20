/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importmailfolderattributejob.h"

class ImportMailFolderAttributeJobImpl : public ImportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobImpl(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJobImpl() override;

protected:
    void applyAttributes() override final;
};
