/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexportercore_private_export.h"
#include <Akonadi/Collection>
#include <QObject>
class KArchiveDirectory;
class ImportMailJobInterface;
class PIMDATAEXPORTER_TESTS_EXPORT ImportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    // TODO remove duplicate code
    struct AttributeInfo {
        QByteArray displayAttribute;
        QByteArray expireAttribute;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !displayAttribute.isEmpty() || !expireAttribute.isEmpty();
        }
    };
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
    virtual void applyAttributes(const QMap<Akonadi::Collection::Id, AttributeInfo> &) = 0;
    void restoreFileFolderAttribute();

private:
    QString mExtractPath;
    const KArchiveDirectory *mArchiveDirectory = nullptr;
    ImportMailJobInterface *mInterface = nullptr;
};
