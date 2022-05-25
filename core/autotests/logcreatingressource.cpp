/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logcreatingressource.h"

#include <QTemporaryFile>
#include <QTextStream>
#include <QVariant>

//#define REMOVE_TEMPORARY_FILE 1

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
        appendText(lst.join(QLatin1Char('\n')));
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
    QMapIterator<QString, QVariant> i(settings);
    while (i.hasNext()) {
        i.next();
        if (i.value().canConvert<QStringList>()) {
            stream << i.key() << ' ' << i.value().toStringList().join(QLatin1Char(',')) << '\n';
        } else {
            stream << i.key() << ' ' << i.value().toString() << '\n';
        }
    }
    stream << "synchronizeTree: " << (synchronizeTree ? "true" : "false") << '\n';
    stream << "--------------------------\n";
    const QString resourceName = resources + QStringLiteral("_new_resource_") + QString::number(mResourceIndex++);
    // qDebug() << " RESOURCE NAME " << resourceName;
    return resourceName;
}
