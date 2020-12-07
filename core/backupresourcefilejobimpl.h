/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BACKUPRESOURCEFILEJOBIMPL_H
#define BACKUPRESOURCEFILEJOBIMPL_H

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

#endif // BACKUPRESOURCEFILEJOBIMPL_H
