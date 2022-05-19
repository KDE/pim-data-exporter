/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "pimdataexportercore_private_export.h"
#include <QObject>
class KJob;
class PIMDATAEXPORTER_TESTS_EXPORT ExportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    explicit ExportMailFolderAttributeJob(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void start();

Q_SIGNALS:
    void finished();

private:
    void slotFetchFinished(KJob *job);
};
