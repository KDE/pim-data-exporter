/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SYNCHRONIZERESOURCEDIALOG_H
#define SYNCHRONIZERESOURCEDIALOG_H

#include <QDialog>
class QListWidget;
class SynchronizeResourceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SynchronizeResourceDialog(QWidget *parent = nullptr);
    ~SynchronizeResourceDialog();

    void setResources(const QHash<QString, QString> &resources);
    Q_REQUIRED_RESULT QStringList resources() const;

private:
    void slotAccepted();
    void slotSelectAll();
    void slotUnselectAll();

    enum DataType {
        ResourceIdentifier = Qt::UserRole + 1
    };

    void readConfig();
    void writeConfig();
    void selectItem(bool state);
    QListWidget *mListResourceWidget = nullptr;
};

#endif // SYNCHRONIZERESOURCEDIALOG_H
