/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loginfile.h"
using namespace Qt::Literals::StringLiterals;

#include "pimdataexportconsole_debug.h"

#include <QFile>

LogInFile::LogInFile(QObject *parent)
    : QObject(parent)
{
}

LogInFile::~LogInFile()
{
    if (mFile) {
        mFile->close();
        delete mFile;
    }
}

QString LogInFile::fileName() const
{
    return mFileName;
}

void LogInFile::setFileName(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        mFileName = fileName;
        if (!mFile) {
            mFile = new QFile(mFileName);
            if (!mFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
                qCWarning(PIMDATAEXPORTERCONSOLE_LOG) << "Impossible to open filename " << mFileName;
                return;
            }
        }
        mTextStream.setDevice(mFile);
    }
}

void LogInFile::addEndLine()
{
    addLogLine(QString(), AddEndLine);
}

void LogInFile::addError(const QString &message)
{
    addLogLine(message, AddError);
}

void LogInFile::addInfo(const QString &message)
{
    addLogLine(message, AddInfo);
}

void LogInFile::addTitle(const QString &message)
{
    addLogLine(message, AddTitle);
}

void LogInFile::addLogLine(const QString &message, LogType type)
{
    QString newMessage;
    switch (type) {
    case AddEndLine:
        newMessage = u'\n';
        break;
    case AddInfo:
        newMessage = u"INFO: %1\n"_s.arg(message);
        break;
    case AddError:
        newMessage = u"ERROR: %1\n"_s.arg(message);
        break;
    case AddTitle:
        newMessage = message + u'\n';
        break;
    }
    mTextStream << newMessage;
}

#include "moc_loginfile.cpp"
