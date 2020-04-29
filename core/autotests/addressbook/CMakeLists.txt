add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportaddressbookjobinterfacetest_SRCS exportaddressbookjobinterfacetest.cpp ../resourceconvertertest.cpp )

add_executable( exportaddressbookjobinterfacetest ${exportaddressbookjobinterfacetest_SRCS})
add_test(NAME exportaddressbookjobinterfacetest COMMAND exportaddressbookjobinterfacetest)
ecm_mark_as_test(exportaddressbookjobinterfacetest)
target_link_libraries(exportaddressbookjobinterfacetest Qt5::Test pimdataexporterprivate KF5::ConfigCore)
