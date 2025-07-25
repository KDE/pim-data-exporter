/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourceconverterbase.h"
using namespace Qt::Literals::StringLiterals;

#include "utils.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KZip>
#include <PimCommon/PimUtil>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>

ResourceConverterBase::ResourceConverterBase() = default;

ResourceConverterBase::~ResourceConverterBase() = default;

QString ResourceConverterBase::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    QString newUrl = ResourceConverterBase::resourcePath(resourceConfig);
    if (!newUrl.contains(installDefaultDirectory())) {
        QFileInfo fileInfo(newUrl);
        fileInfo.fileName();
        // qCDebug(PIMDATAEXPORTERCORE_LOG)<<" url "<<url.path();
        QString currentPath = installDefaultDirectory() + u'/' + storedData;
        newUrl = (currentPath + u'/' + fileInfo.fileName());
        if (!QDir(currentPath).exists()) {
            if (!QDir().mkpath(currentPath)) {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to create subpath " << currentPath;
            }
        }
    }
    if (QFileInfo::exists(newUrl)) {
        QString newFileName = newUrl;
        QFileInfo fileInfo(newFileName);
        for (int i = 0;; ++i) {
            const QString currentPath = fileInfo.path() + u'/' + QString::number(i) + u'/';
            newFileName = currentPath + fileInfo.fileName();
            if (!QFileInfo::exists(newFileName)) {
                if (!QDir().mkpath(currentPath)) {
                    qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to create subpath " << currentPath;
                }
                break;
            }
        }
        newUrl = newFileName;
    }
    return newUrl;
}

QString ResourceConverterBase::resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath)
{
    KConfigGroup group = resourceConfig->group(u"General"_s);
    QString url = group.readEntry(u"Path"_s, defaultPath);
    if (!url.isEmpty()) {
        url.replace(QLatin1StringView("$HOME"), QDir::homePath());
    }
    url = changeResourcePath(url);
    return url;
}

QString ResourceConverterBase::resourcePath(const QString &agentIdentifier, const QString &defaultPath)
{
    const QString agentFileName = agentIdentifier + u"rc"_s;
    const QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + agentFileName;
    // qDebug() << "configFileName " << configFileName;

    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(configFileName);
    const QString url = resourcePath(resourceConfig, defaultPath);
    // qDebug() << " resourcePath " << url;
    return url;
}

QString ResourceConverterBase::changeResourcePath(QString url) const
{
    return url;
}

// Merge two methods I think
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
                collection.remove(u'c');
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
            collectionId.remove(u'c');
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
    agentFileConfigName.remove(agentFileConfigName.length() - 2, 2); // Remove "rc"
    agentFileConfigName = Utils::resourcesPath() + Utils::prefixAkonadiConfigFile() + agentFileConfigName;
    return agentFileConfigName;
}

QString ResourceConverterBase::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    const QString agentFileName = identifier + u"rc"_s;
    const QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + agentFileName;
    qCWarning(PIMDATAEXPORTERCORE_LOG) << "configFileName " << configFileName << "agentFileName " << configFileName;

    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(configFileName);
    QTemporaryFile tmp;
    tmp.open();
    KConfig *config = resourceConfig->copyTo(tmp.fileName());

    if (identifier.contains(POP3_RESOURCE_IDENTIFIER)) {
        const QString targetCollection = u"targetCollection"_s;
        KConfigGroup group = config->group(u"General"_s);
        if (group.hasKey(targetCollection)) {
            group.writeEntry(targetCollection, convertToFullCollectionPath(group.readEntry(targetCollection).toLongLong()));
        }
    } else if (PimCommon::Util::isImapResource(identifier)) {
        const QString trash = u"TrashCollection"_s;
        KConfigGroup group = config->group(u"cache"_s);
        if (group.hasKey(trash)) {
            group.writeEntry(trash, convertToFullCollectionPath(group.readEntry(trash).toLongLong()));
        }
    }
    // Customize resource if necessary here.
    config->sync();
    bool fileAdded = archive->addLocalFile(tmp.fileName(), path + agentFileName);
    delete config;
    if (!fileAdded) {
        return i18n("Resource file \"%1\" cannot be added to backup file.", agentFileName);
    }

    const QString agentConfigFileName = Utils::prefixAkonadiConfigFile() + identifier;
    const QString agentConfigFileNamePath =
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1StringView("/akonadi/") + agentConfigFileName;
    if (QFileInfo::exists(agentConfigFileNamePath)) {
        fileAdded = archive->addLocalFile(agentConfigFileNamePath, path + agentConfigFileName);
        if (!fileAdded) {
            return i18n("Resource file \"%1\" cannot be added to backup file.", agentConfigFileNamePath);
        }
    } else {
        return i18n("Resource config file \"%1\" doesn't exist.", agentConfigFileNamePath);
    }

    return {};
}
