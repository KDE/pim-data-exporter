/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedback/ApplicationVersionSource>
#include <KUserFeedback/LocaleInfoSource>
#include <KUserFeedback/PlatformInfoSource>
#include <KUserFeedback/QtVersionSource>
#include <KUserFeedback/ScreenInfoSource>
#include <KUserFeedback/StartCountSource>
#include <KUserFeedback/UsageTimeSource>

PimDataExportedUserFeedbackProvider::PimDataExportedUserFeedbackProvider(QObject *parent)
    : KUserFeedback::Provider(parent)
{
    setProductIdentifier(QStringLiteral("org.kde.pim-data-exporter"));
    setFeedbackServer(QUrl(QStringLiteral("https://telemetry.kde.org/")));
    setSubmissionInterval(7);
    setApplicationStartsUntilEncouragement(5);
    setEncouragementDelay(30);

    addDataSource(new KUserFeedback::ApplicationVersionSource);
    addDataSource(new KUserFeedback::PlatformInfoSource);
    addDataSource(new KUserFeedback::ScreenInfoSource);
    addDataSource(new KUserFeedback::QtVersionSource);

    addDataSource(new KUserFeedback::StartCountSource);
    addDataSource(new KUserFeedback::UsageTimeSource);

    addDataSource(new KUserFeedback::LocaleInfoSource);
}

PimDataExportedUserFeedbackProvider::~PimDataExportedUserFeedbackProvider() = default;

#include "moc_pimdataexporteduserfeedbackprovider.cpp"
