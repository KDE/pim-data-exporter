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

#include "logcreatingressource.h"

#include <QTemporaryFile>
#include <QTextStream>
#include <QDebug>

LogCreatingResource::LogCreatingResource(QObject *parent)
    : QObject(parent)
{
    mTmpLogFile = new QTemporaryFile(this);
    mTmpLogFile->open();
}

LogCreatingResource::~LogCreatingResource()
{
}

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
        stream << i.key() << ' ' << i.value().toString() << '\n';
    }
    stream << synchronizeTree << '\n';
    stream << "--------------------------\n";
    return resources + QStringLiteral("_new_resource_") + QString::number(mResourceIndex++);
}
