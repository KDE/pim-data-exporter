# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(
    ${pim-data-exporter_BINARY_DIR}/console/
)

ecm_qt_declare_logging_category(pimdataexporterconsole_loginfiletest_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole)



add_executable(pimdataexporterconsole_loginfiletest loginfiletest.cpp loginfiletest.h ../loginfile.cpp ${pimdataexporterconsole_loginfiletest_SRCS})
add_test(NAME pimdataexporterconsole_loginfiletest COMMAND pimdataexporterconsole_loginfiletest)
ecm_mark_as_test(pimdataexporterconsole_loginfiletest)
target_link_libraries(pimdataexporterconsole_loginfiletest Qt::Test)

