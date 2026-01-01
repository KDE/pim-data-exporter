/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logcreatingressource.h"
using namespace Qt::Literals::StringLiterals;

#include <QTemporaryFile>
#include <QTextStream>
#include <QVariant>

// #define REMOVE_TEMPORARY_FILE 1

LogCreatingResource::LogCreatingResource(QObject *parent)
    : QObject(parent)
    , mTmpLogFile(new QTemporaryFile(this))
{
#ifdef REMOVE_TEMPORARY_FILE
    mTmpLogFile->setAutoRemove(true);
#else
    mTmpLogFile->setAutoRemove(false);
#endif
    mTmpLogFile->open();
    // qDebug() << " mTmpLogFile*************" << mTmpLogFile->fileName();
}

LogCreatingResource::~LogCreatingResource() = default;

void LogCreatingResource::appendText(const QString &str)
{
    QTextStream stream(mTmpLogFile);
    stream << str << '\n';
    stream << "--------------------------\n";
}

void LogCreatingResource::logSynchronizeResource(const QStringList &lst)
{
    if (!lst.isEmpty()) {
        appendText(lst.join(u'\n'));
    }
}

void LogCreatingResource::logRegisterSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    QTextStream stream(mTmpLogFile);
    stream << "Special Collection : " << type << " colId " << colId << '\n';
    stream << "--------------------------\n";
}

QString LogCreatingResource::logPath() const
{
    return mTmpLogFile->fileName();
}

QString LogCreatingResource::logCreateResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    QTextStream stream(mTmpLogFile);
    stream << resources << '\n';
    stream << name << '\n';
    for (const auto &[key, value] : settings.asKeyValueRange()) {
        if (value.canConvert<QStringList>()) {
            stream << key << ' ' << value.toStringList().join(u',') << '\n';
        } else {
            stream << key << ' ' << value.toString() << '\n';
        }
    }
    stream << "synchronizeTree: " << (synchronizeTree ? "true" : "false") << '\n';
    stream << "--------------------------\n";
    const QString resourceName = resources + u"_new_resource_"_s + QString::number(mResourceIndex++);
    // qDebug() << " RESOURCE NAME " << resourceName;
    return resourceName;
}

#include "moc_logcreatingressource.cpp"
