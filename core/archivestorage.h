/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "pimdataexporter_export.h"
#include <QObject>
class KZip;

class PIMDATAEXPORTER_EXPORT ArchiveStorage : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveStorage(const QString &filename, QObject *parent = nullptr);
    ~ArchiveStorage() override;

    void closeArchive();
    [[nodiscard]] bool openArchive(bool write);

    KZip *archive() const;
    [[nodiscard]] QString filename() const;

Q_SIGNALS:
    void error(const QString &);

private:
    KZip *const mArchive;
};
