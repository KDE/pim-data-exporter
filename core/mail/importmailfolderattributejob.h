/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexportercore_private_export.h"
#include <QObject>
class PIMDATAEXPORTER_TESTS_EXPORT ImportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJob(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void start();
};
