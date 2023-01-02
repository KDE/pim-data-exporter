/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importexportmailutil.h"
#include "pimdataexportercore_private_export.h"
#include <Akonadi/Collection>
#include <QObject>
class KArchiveDirectory;
class ImportMailJobInterface;
class PIMDATAEXPORTER_TESTS_EXPORT ImportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJob(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void setArchiveDirectory(const KArchiveDirectory *zip);
    void setExportInterface(ImportMailJobInterface *interface);
    void setExtractPath(const QString &extractPath);
    void start();

Q_SIGNALS:
    void successed();
    void failed();

protected:
    virtual void applyAttributes(const QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> &) = 0;
    void restoreFileFolderAttribute();
    QString mExtractPath;

private:
    const KArchiveDirectory *mArchiveDirectory = nullptr;
    ImportMailJobInterface *mInterface = nullptr;
};
