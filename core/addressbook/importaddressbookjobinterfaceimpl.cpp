/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "importaddressbookjobinterfaceimpl.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KZip>
#include <KArchiveEntry>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "resourceconverterimpl.h"

namespace {
inline const QString storeAddressbook()
{
    return QStringLiteral("backupaddressbook/");
}
}

ImportAddressbookJobInterfaceImpl::ImportAddressbookJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAddressbookJobInterfaceImpl::~ImportAddressbookJobInterfaceImpl()
{
}

void ImportAddressbookJobInterfaceImpl::restoreResources()
{
    Q_EMIT info(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    increaseProgressDialog();
    QStringList listResource;
    listResource
        << restoreResourceFile(QStringLiteral("akonadi_vcard_resource"), Utils::addressbookPath(), QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral(
                                   "/share/kaddressbook/"));

    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::addressbookPath());
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + Utils::addressbookPath());

        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            resourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(QLatin1String("akonadi_vcarddir_resource_"))
                || value.akonadiConfigFile.contains(QLatin1String("akonadi_contacts_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const KArchiveFile *file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + QLatin1Char('/') + resourceName);

                    //TODO fix default path
                    ResourceConverterImpl converter;
                    const QString newUrl = converter.adaptResourcePath(resourceConfig, QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/contacts"));
                    QFileInfo newUrlInfo(newUrl);
                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    if (dataResouceEntry->isFile()) {
                        const KArchiveFile *file = static_cast<const KArchiveFile *>(dataResouceEntry);
                        //TODO  adapt directory name too
                        extractZipFile(file, copyToDirName, newUrlInfo.path(), value.akonadiConfigFile.contains(QLatin1String("akonadi_contacts_resource_")));
                    }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const KArchiveFile *file = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(file, copyToDirName);
                            resourceName = file->name();
                            filename = Utils::akonadiAgentName(copyToDirName + QLatin1Char('/') + resourceName);
                        }
                    }
                    QString instanceType;
                    if (value.akonadiConfigFile.contains(QLatin1String("akonadi_vcarddir_resource_"))) {
                        instanceType = QStringLiteral("akonadi_vcarddir_resource");
                    } else if (value.akonadiConfigFile.contains(QLatin1String("akonadi_contacts_resource_"))) {
                        instanceType = QStringLiteral("akonadi_contacts_resource");
                    } else {
                        qCWarning(PIMDATAEXPORTERCORE_LOG) << " not supported" << value.akonadiConfigFile;
                    }

                    const QString newResource = mCreateResource->createResource(instanceType, filename, settings, true);
                    infoAboutNewResource(newResource);
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                    listResource << newResource;
                }
            }
        }
    }

    Q_EMIT info(i18n("Resources restored."));
    //It's maildir support. Need to add support
    startSynchronizeResources(listResource);
}
