#! /bin/sh
$EXTRACTRC `find . -name '*.ui' -or -name '*.rc' | grep -v '/autotests/'` >> rc.cpp || exit 11
$XGETTEXT `find -name '*.cpp' | grep -v '/tests/'` -o $podir/pimdataexporter.pot
rm -f rc.cpp
