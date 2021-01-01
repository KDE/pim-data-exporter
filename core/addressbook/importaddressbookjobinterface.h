/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTADDRESSBOOKJOBINTERFACE_H
#define IMPORTADDRESSBOOKJOBINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;
class KArchiveFile;

class PIMDATAEXPORTER_TESTS_EXPORT ImportAddressbookJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAddressbookJobInterface() override;

    void start() override;

protected:
    void slotNextStep() override;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    virtual Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;
    virtual Q_REQUIRED_RESULT QString configLocation() const;
    void restoreResources();
private:
    Q_REQUIRED_RESULT bool isAConfigFile(const QString &name) const override;
    void importkaddressBookConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void restoreConfig();
    void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings) override;
    Q_REQUIRED_RESULT QString installConfigLocation() const;
};

#endif // IMPORTADDRESSBOOKJOBINTERFACE_H
