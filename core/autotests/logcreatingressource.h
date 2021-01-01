/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOGCREATINGRESOURCE_H
#define LOGCREATINGRESOURCE_H

#include <QObject>
#include <Akonadi/KMime/SpecialMailCollections>
class QTemporaryFile;
class LogCreatingResource : public QObject
{
    Q_OBJECT
public:
    explicit LogCreatingResource(QObject *parent = nullptr);
    ~LogCreatingResource() override;
    void appendText(const QString &str);
    Q_REQUIRED_RESULT QString logPath() const;
    Q_REQUIRED_RESULT QString logCreateResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree);
    void logSynchronizeResource(const QStringList &lst);
    void logRegisterSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId);
private:
    QTemporaryFile *mTmpLogFile = nullptr;
    int mResourceIndex = 0;
};

#endif // LOGCREATINGRESOURCE_H
