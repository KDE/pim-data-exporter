/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
namespace PimCommon {
class CustomLogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent);
    ~LogWidget() override;

    void addInfoLogEntry(const QString &log);
    void addErrorLogEntry(const QString &log);
    void addTitleLogEntry(const QString &log);
    void addEndLineLogEntry();
    void clear();
    Q_REQUIRED_RESULT QString toHtml() const;
    Q_REQUIRED_RESULT QString toPlainText() const;
    Q_REQUIRED_RESULT bool isEmpty() const;

private:
    PimCommon::CustomLogWidget *mCustomLogWidget = nullptr;
};

#endif // LOGWIDGET_H
