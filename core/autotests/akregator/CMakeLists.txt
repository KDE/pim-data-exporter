# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )


set( exportakregatorjobinterfacetest_SRCS exportakregatorjobinterfacetest.cpp exportakregatorjobinterfacetestimpl.cpp
    exportakregatorjobinterfacetest.h exportakregatorjobinterfacetestimpl.h)

add_executable( exportakregatorjobinterfacetest ${exportakregatorjobinterfacetest_SRCS})
add_test(NAME exportakregatorjobinterfacetest COMMAND exportakregatorjobinterfacetest)
ecm_mark_as_test(exportakregatorjobinterfacetest)
target_link_libraries(exportakregatorjobinterfacetest Qt::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)

#####

set( importakregatorjobinterfacetest_SRCS importakregatorjobinterfacetest.cpp importakregatorjobinterfacetestimpl.cpp
    importakregatorjobinterfacetest.h importakregatorjobinterfacetestimpl.h)

add_executable( importakregatorjobinterfacetest ${importakregatorjobinterfacetest_SRCS})
add_test(NAME importakregatorjobinterfacetest COMMAND importakregatorjobinterfacetest)
ecm_mark_as_test(importakregatorjobinterfacetest)
target_link_libraries(importakregatorjobinterfacetest Qt::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
