/*
  Copyright (c) 2012-2013 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ABSTRACTIMPORTEXPORTJOB_H
#define ABSTRACTIMPORTEXPORTJOB_H

#include "utils.h"
#include <Akonadi/Collection>

#include <KZip>

class QWidget;
class QProgressDialog;
class ArchiveStorage;
class KArchiveDirectory;
class KTempDir;

namespace KPIMIdentities {
class Identity;
class IdentityManager;
}

namespace PimCommon {
class CreateResource;
}

class AbstractImportExportJob : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImportExportJob(QWidget *parent, ArchiveStorage *archiveStorage, Utils::StoredTypes typeSelected, int numberOfStep);
    ~AbstractImportExportJob();

    virtual void start() = 0;

    bool wasCanceled() const;

Q_SIGNALS:
    void info(const QString &);
    void error(const QString &);

protected:
    void convertRealPathToCollection(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix=false);
    void convertRealPathToCollectionList(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix=true);
    void copyToFile(const KArchiveFile * archivefile, const QString &dest, const QString &filename, const QString &prefix);
    void initializeImportJob();
    void backupFile(const QString &filename, const QString &path, const QString &storedName);
    int mergeConfigMessageBox(const QString &configName) const;
    bool overwriteConfigMessageBox(const QString &configName) const;
    Akonadi::Collection::Id convertPathToId(const QString &path);
    void backupResourceFile(const Akonadi::AgentInstance &agent, const QString &defaultPath);

    KZip *archive();

    QProgressDialog *progressDialog();
    void increaseProgressDialog();
    void createProgressDialog();

    void showInfo(const QString &text);

    QHash<QString, Akonadi::Collection::Id> mHashConvertPathCollectionId;
    QList<resourceFiles> mListResourceFile;

    QString mTempDirName;
    Utils::StoredTypes mTypeSelected;
    ArchiveStorage *mArchiveStorage;
    KPIMIdentities::IdentityManager *mIdentityManager;
    QWidget *mParent;
    KTempDir *mTempDir;
    QProgressDialog *mProgressDialog;
    const KArchiveDirectory* mArchiveDirectory;
    int mNumberOfStep;
    PimCommon::CreateResource *mCreateResource;
};

#endif // ABSTRACTIMPORTEXPORTJOB_H
