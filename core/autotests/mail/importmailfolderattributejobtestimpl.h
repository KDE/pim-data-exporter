/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mail/importexportmailutil.h"
#include "mail/importmailfolderattributejob.h"

class ImportMailFolderAttributeJobTestImpl : public ImportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobTestImpl(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJobTestImpl() override;

protected:
    void applyAttributes(const QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> &) override final;
};
