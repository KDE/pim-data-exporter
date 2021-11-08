/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "backupresourcefilejobbase.h"

#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT BackupResourceFileJobImpl : public BackupResourceFileJobBase
{
    Q_OBJECT
public:
    explicit BackupResourceFileJobImpl(QObject *parent = nullptr);
    ~BackupResourceFileJobImpl() override;

    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
};
