/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdatabackuprestore.h"

class PimDataBackupRestoreUI : public PimDataBackupRestore
{
    Q_OBJECT
public:
    explicit PimDataBackupRestoreUI(QWidget *parentWidget, QObject *parent = nullptr);
    ~PimDataBackupRestoreUI() override;

protected:
    Q_REQUIRED_RESULT bool continueToRestore() override;
    void addExportProgressIndicator() override;

private:
    QWidget *const mParentWidget;
};
