/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourceconverterclasstest.h"
using namespace Qt::Literals::StringLiterals;

#include "../utils.h"
#include "resourceconvertertest.h"
#include <QTest>
QTEST_MAIN(ResourceConverterClassTest)

ResourceConverterClassTest::ResourceConverterClassTest(QObject *parent)
    : QObject(parent)
{
}

void ResourceConverterClassTest::shouldGetAgentFileName()
{
    ResourceConverterTest w;
    const QString filename = Utils::resourcesPath() + u"akonadi_imap_resource_0rc"_s;
    QCOMPARE(w.agentFileName(filename), u"resources/agent_config_akonadi_imap_resource_0"_s);
}

#include "moc_resourceconverterclasstest.cpp"
