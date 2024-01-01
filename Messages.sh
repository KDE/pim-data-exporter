#! /bin/sh
# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org> 
$EXTRACTRC `find . -name '*.ui' -or -name '*.rc' | grep -v '/autotests/'` >> rc.cpp || exit 11
$XGETTEXT `find -name '*.cpp' | grep -v '/tests/'` -o $podir/pimdataexporter.pot
rm -f rc.cpp
