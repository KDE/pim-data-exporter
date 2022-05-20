/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "pimdataexportercore_private_export.h"
#include <QObject>
class KJob;
class KZip;
class ExportMailJobInterface;
class PIMDATAEXPORTER_TESTS_EXPORT ExportMailFolderAttributeJob : public QObject
{
    Q_OBJECT
public:
    struct AttributeInfo {
        QByteArray displayAttribute;
        QByteArray expireAttribute;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !displayAttribute.isEmpty() || !expireAttribute.isEmpty();
        }
    };

    explicit ExportMailFolderAttributeJob(QObject *parent = nullptr);
    ~ExportMailFolderAttributeJob() override;

    Q_REQUIRED_RESULT bool canStart() const;

    void start();
    void setArchive(KZip *zip);
    void setExportInterface(ExportMailJobInterface *interface);

Q_SIGNALS:
    void successed();
    void failed();

protected:
    virtual void fetchAttributes() = 0;

    void storeFileFolderAttribute(const QMap<QString, AttributeInfo> &lstAttributeInfo);

    KZip *mArchive = nullptr;
    ExportMailJobInterface *mInterface = nullptr;
};
