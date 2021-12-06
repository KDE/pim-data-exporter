# SPDX-FileCopyrightText: 2011-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportaddressbookjobinterfacetest_SRCS exportaddressbookjobinterfacetest.cpp exportaddressbookjobinterfacetestimpl.cpp)

add_executable( exportaddressbookjobinterfacetest ${exportaddressbookjobinterfacetest_SRCS})
add_test(NAME exportaddressbookjobinterfacetest COMMAND exportaddressbookjobinterfacetest)
ecm_mark_as_test(exportaddressbookjobinterfacetest)
target_link_libraries(exportaddressbookjobinterfacetest Qt::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
#####

set( importaddressbookjobinterfacetest_SRCS importaddressbookjobinterfacetest.cpp importaddressbookjobinterfacetestimpl.cpp)

add_executable( importaddressbookjobinterfacetest ${importaddressbookjobinterfacetest_SRCS})
add_test(NAME importaddressbookjobinterfacetest COMMAND importaddressbookjobinterfacetest)
ecm_mark_as_test(importaddressbookjobinterfacetest)
target_link_libraries(importaddressbookjobinterfacetest Qt::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
