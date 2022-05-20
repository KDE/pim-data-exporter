/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejob.h"
#include "pimdataexportcore_debug.h"

ImportMailFolderAttributeJob::ImportMailFolderAttributeJob(QObject *parent)
    : QObject{parent}
{
}

ImportMailFolderAttributeJob::~ImportMailFolderAttributeJob()
{
}

bool ImportMailFolderAttributeJob::canStart() const
{
    return (mArchive != nullptr);
}

void ImportMailFolderAttributeJob::start()
{
    if (!canStart()) {
        Q_EMIT failed();
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " Impossible to start job";
        deleteLater();
        return;
    }
    // TODO
}
