/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KZip>

class SaveResourceConfigTest
{
public:
    SaveResourceConfigTest();
    KZip *archive() const;
    void setArchive(KZip *archive);

    void exportResourceToArchive(const QString &archivePath,
                                 const QString &url,
                                 const QString &identifier,
                                 const QString &resourceArchiveName,
                                 const QStringList &resourceIdentifierTypes);

private:
    KZip *mArchive = nullptr;
};
