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

#include "resourceconverterbase.h"
#include "utils.h"
#include <PimCommon/PimUtil>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>

ResourceConverterBase::ResourceConverterBase()
{
}

ResourceConverterBase::~ResourceConverterBase()
{
}

QString ResourceConverterBase::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    QString newUrl = ResourceConverterBase::resourcePath(resourceConfig);
    if (!newUrl.contains(installDefaultDirectory())) {
        QFileInfo fileInfo(newUrl);
        fileInfo.fileName();
        //qCDebug(PIMDATAEXPORTERCORE_LOG)<<" url "<<url.path();
        QString currentPath = installDefaultDirectory() + QLatin1Char('/') + storedData;
        newUrl = (currentPath + QLatin1Char('/') + fileInfo.fileName());
        if (!QDir(currentPath).exists()) {
            QDir().mkdir(currentPath);
        }
    }
    if (QFileInfo::exists(newUrl)) {
        QString newFileName = newUrl;
        QFileInfo fileInfo(newFileName);
        for (int i = 0;; ++i) {
            const QString currentPath = fileInfo.path() + QLatin1Char('/') + QString::number(i) + QLatin1Char('/');
            newFileName = currentPath + fileInfo.fileName();
            if (!QFileInfo::exists(newFileName)) {
                QDir().mkdir(currentPath);
                break;
            }
        }
        newUrl = newFileName;
    }
    return newUrl;
}

QString ResourceConverterBase::resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath)
{
    KConfigGroup group = resourceConfig->group(QStringLiteral("General"));
    QString url = group.readEntry(QStringLiteral("Path"), defaultPath);
    if (!url.isEmpty()) {
        url.replace(QLatin1String("$HOME"), QDir::homePath());
    }
    url = changeResourcePath(url);
    return url;
}

QString ResourceConverterBase::resourcePath(const QString &agentIdentifier, const QString &defaultPath)
{
    const QString agentFileName = agentIdentifier + QStringLiteral("rc");
    const QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + agentFileName;
    qDebug() << "configFileName " << configFileName;

    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(configFileName);
    const QString url = resourcePath(resourceConfig, defaultPath);
    qDebug() << " resourcePath " << url;
    return url;
}

QString ResourceConverterBase::changeResourcePath(QString url) const
{
    return url;
}

//Merge two methods I think
void ResourceConverterBase::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    if (group.hasKey(currentKey)) {
        const QStringList value = group.readEntry(currentKey, QStringList());
        QStringList newValue;
        for (QString str : value) {
            bool found = false;
            if (!prefixCollection.isEmpty() && str.startsWith(prefixCollection)) {
                str.remove(0, prefixCollection.length());
            }
            const qlonglong collectionId = str.toLongLong(&found);
            if (found) {
                const QString realPath = convertToFullCollectionPath(collectionId);
                if (!realPath.isEmpty()) {
                    newValue << realPath;
                }
            }
        }
        if (newValue.isEmpty()) {
            group.deleteEntry(currentKey);
        } else {
            group.writeEntry(currentKey, newValue);
        }
    }
}

void ResourceConverterBase::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    if (group.hasKey(currentKey)) {
        const QStringList listExpension = group.readEntry(currentKey, QStringList());
        if (listExpension.isEmpty()) {
            group.deleteEntry(currentKey);
        } else {
            QStringList result;
            for (QString collection : listExpension) {
                collection.remove(QLatin1Char('c'));
                bool found = false;
                const qlonglong collectionValue = collection.toLongLong(&found);
                if (found && collectionValue != -1) {
                    const QString realPath = convertToFullCollectionPath(collectionValue);
                    if (!realPath.isEmpty()) {
                        result << realPath;
                    }
                }
            }
            if (result.isEmpty()) {
                group.deleteEntry(currentKey);
            } else {
                group.writeEntry(currentKey, result);
            }
        }
    }
}

void ResourceConverterBase::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    if (group.hasKey(currentKey)) {
        QString collectionId = group.readEntry(currentKey);
        if (collectionId.isEmpty()) {
            group.deleteEntry(currentKey);
        } else {
            collectionId.remove(QLatin1Char('c'));
            bool found = false;
            const qlonglong collectionValue = collectionId.toLongLong(&found);
            if (found && collectionValue != -1) {
                const QString realPath = convertToFullCollectionPath(collectionValue);
                group.writeEntry(currentKey, realPath);
            } else {
                group.deleteEntry(currentKey);
            }
        }
    }
}

QString ResourceConverterBase::agentFileName(const QString &filename)
{
    QString agentFileConfigName = filename;
    agentFileConfigName.remove(Utils::resourcesPath());
    agentFileConfigName.remove(agentFileConfigName.length() - 2, 2); //Remove "rc"
    agentFileConfigName = Utils::resourcesPath() + Utils::prefixAkonadiConfigFile() + agentFileConfigName;
    return agentFileConfigName;
}

QString ResourceConverterBase::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    const QString agentFileName = identifier + QStringLiteral("rc");
    const QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + agentFileName;
    qCDebug(PIMDATAEXPORTERCORE_LOG) << "configFileName " << configFileName << "agentFileName " << configFileName;

    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(configFileName);
    QTemporaryFile tmp;
    tmp.open();
    KConfig *config = resourceConfig->copyTo(tmp.fileName());

    if (identifier.contains(POP3_RESOURCE_IDENTIFIER)) {
        const QString targetCollection = QStringLiteral("targetCollection");
        KConfigGroup group = config->group("General");
        if (group.hasKey(targetCollection)) {
            group.writeEntry(targetCollection, convertToFullCollectionPath(group.readEntry(targetCollection).toLongLong()));
        }
    } else if (PimCommon::Util::isImapResource(identifier)) {
        const QString trash = QStringLiteral("TrashCollection");
        KConfigGroup group = config->group("cache");
        if (group.hasKey(trash)) {
            group.writeEntry(trash, convertToFullCollectionPath(group.readEntry(trash).toLongLong()));
        }
    }
    //Customize resource if necessary here.
    config->sync();
    bool fileAdded = archive->addLocalFile(tmp.fileName(), path + agentFileName);
    delete config;
    if (!fileAdded) {
        return i18n("Resource file \"%1\" cannot be added to backup file.", agentFileName);
    }

    const QString agentConfigFileName = Utils::prefixAkonadiConfigFile() + identifier;
    const QString agentConfigFileNamePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1String("/akonadi/") + agentConfigFileName;
    if (QFileInfo::exists(agentConfigFileNamePath)) {
        fileAdded = archive->addLocalFile(agentConfigFileNamePath, path + agentConfigFileName);
        if (!fileAdded) {
            return i18n("Resource file \"%1\" cannot be added to backup file.", agentFileName);
        }
    }

    return QString();
}
