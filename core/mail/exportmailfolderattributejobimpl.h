/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportmailfolderattributejob.h"
class KJob;
class ExportMailFolderAttributeJobImpl : public ExportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ExportMailFolderAttributeJobImpl(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJobImpl() override;

protected:
    void fetchAttributes() override final;

private:
    void slotFetchFinished(KJob *job);
};
