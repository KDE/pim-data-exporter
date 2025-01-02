/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString resourcePath(const QString &identifier) const override;
};
