set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ${pim-data-exporter_BINARY_DIR}/console/
    )

set( pimdataexporterconsole_loginfiletest_SRCS loginfiletest.cpp ../loginfile.cpp )

ecm_qt_declare_logging_category(pimdataexporterconsole_loginfiletest_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole)



add_executable( pimdataexporterconsole_loginfiletest ${pimdataexporterconsole_loginfiletest_SRCS})
add_test(NAME pimdataexporterconsole_loginfiletest COMMAND pimdataexporterconsole_loginfiletest)
ecm_mark_as_test(pimdataexporterconsole_loginfiletest)
target_link_libraries( pimdataexporterconsole_loginfiletest Qt5::Test)

