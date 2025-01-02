/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
namespace PimCommon
{
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
    [[nodiscard]] QString toHtml() const;
    [[nodiscard]] QString toPlainText() const;
    [[nodiscard]] bool isEmpty() const;

private:
    PimCommon::CustomLogWidget *const mCustomLogWidget;
};
