/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logwidget.h"

#include <PimCommon/CustomLogWidget>

#include <QHBoxLayout>

LogWidget::LogWidget(QWidget *parent)
    : QWidget(parent)
{
  auto layout = new QHBoxLayout(this);
  mCustomLogWidget = new PimCommon::CustomLogWidget(this);
  layout->addWidget(mCustomLogWidget);
}

LogWidget::~LogWidget()
{
}

void LogWidget::clear()
{
    mCustomLogWidget->clear();
}

void LogWidget::addInfoLogEntry(const QString &log)
{
    mCustomLogWidget->addInfoLogEntry(log);
}

void LogWidget::addErrorLogEntry(const QString &log)
{
    mCustomLogWidget->addErrorLogEntry(log);
}

void LogWidget::addTitleLogEntry(const QString &log)
{
    mCustomLogWidget->addTitleLogEntry(log);
}

QString LogWidget::toHtml() const
{
    return mCustomLogWidget->toHtml();
}

QString LogWidget::toPlainText() const
{
    return mCustomLogWidget->toPlainText();
}

bool LogWidget::isEmpty() const
{
    return mCustomLogWidget->isEmpty();
}

void LogWidget::addEndLineLogEntry()
{
    mCustomLogWidget->addEndLineLogEntry();
}
