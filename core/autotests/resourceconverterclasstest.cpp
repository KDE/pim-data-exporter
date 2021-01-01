/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourceconverterclasstest.h"
#include "resourceconvertertest.h"
#include "../utils.h"
#include <QTest>
QTEST_MAIN(ResourceConverterClassTest)

ResourceConverterClassTest::ResourceConverterClassTest(QObject *parent)
    : QObject(parent)
{
}

void ResourceConverterClassTest::shouldGetAgentFileName()
{
    ResourceConverterTest w;
    const QString filename = Utils::resourcesPath() + QStringLiteral("akonadi_imap_resource_0rc");
    QCOMPARE(w.agentFileName(filename), QStringLiteral("resources/agent_config_akonadi_imap_resource_0"));
}
