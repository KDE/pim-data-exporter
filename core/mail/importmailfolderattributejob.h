/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexportercore_private_export.h"
#include <QObject>
class KZip;
class ImportMailJobInterface;
class PIMDATAEXPORTER_TESTS_EXPORT ImportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJob(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void setArchive(KZip *zip);
    void setExportInterface(ImportMailJobInterface *interface);

    void start();

Q_SIGNALS:
    void successed();
    void failed();

protected:
    virtual void applyAttributes() = 0;
    void restoreFileFolderAttribute();

private:
    KZip *mArchive = nullptr;
    ImportMailJobInterface *mInterface = nullptr;
};
