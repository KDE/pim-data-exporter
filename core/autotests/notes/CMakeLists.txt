# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )


set( exportnotesjobinterfacetest_SRCS exportnotesjobinterfacetest.cpp exportnotesjobinterfacetestimpl.cpp
    exportnotesjobinterfacetest.h exportnotesjobinterfacetestimpl.h)

add_executable( exportnotesjobinterfacetest ${exportnotesjobinterfacetest_SRCS})
add_test(NAME exportnotesjobinterfacetest COMMAND exportnotesjobinterfacetest)
ecm_mark_as_test(exportnotesjobinterfacetest)
target_link_libraries( exportnotesjobinterfacetest Qt::Test pimdataexporterprivate test_resourceconverter KF6::ConfigCore KF6::Archive KPim6::AkonadiCore)
#######

set( importnotesjobinterfacetest_SRCS importnotesjobinterfacetest.cpp importnotesjobinterfacetestimpl.cpp
    importnotesjobinterfacetest.h importnotesjobinterfacetestimpl.h)

add_executable( importnotesjobinterfacetest ${importnotesjobinterfacetest_SRCS})
add_test(NAME importnotesjobinterfacetest COMMAND importnotesjobinterfacetest)
ecm_mark_as_test(importnotesjobinterfacetest)
target_link_libraries( importnotesjobinterfacetest Qt::Test pimdataexporterprivate test_resourceconverter KF6::ConfigCore KF6::Archive)
