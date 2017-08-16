set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ${pim-data-exporter_BINARY_DIR}/console/
    )

set( pimsettingexporterconsole_loginfiletest_SRCS loginfiletest.cpp ../loginfile.cpp )

ecm_qt_declare_logging_category(pimsettingexporterconsole_loginfiletest_SRCS HEADER pimsettingexportconsole_debug.h IDENTIFIER PIMSETTINGEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimsettingexporterconsole)


add_executable( pimsettingexporterconsole_loginfiletest ${pimsettingexporterconsole_loginfiletest_SRCS})
add_test(NAME pimsettingexporterconsole_loginfiletest COMMAND pimsettingexporterconsole_loginfiletest)
ecm_mark_as_test(pimsettingexporterconsole_loginfiletest)
target_link_libraries( pimsettingexporterconsole_loginfiletest Qt5::Test)

