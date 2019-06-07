set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ${pim-data-exporter_BINARY_DIR}/console/
    )

set( PIMDATAEXPORTERCONSOLE_LOGinfiletest_SRCS loginfiletest.cpp ../loginfile.cpp )

ecm_qt_declare_logging_category(PIMDATAEXPORTERCONSOLE_LOGinfiletest_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole)


add_executable( PIMDATAEXPORTERCONSOLE_LOGinfiletest ${PIMDATAEXPORTERCONSOLE_LOGinfiletest_SRCS})
add_test(NAME PIMDATAEXPORTERCONSOLE_LOGinfiletest COMMAND PIMDATAEXPORTERCONSOLE_LOGinfiletest)
ecm_mark_as_test(PIMDATAEXPORTERCONSOLE_LOGinfiletest)
target_link_libraries( PIMDATAEXPORTERCONSOLE_LOGinfiletest Qt5::Test)

