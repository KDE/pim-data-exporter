add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportmailjobinterfacetest_SRCS exportmailjobinterfacetest.cpp ../resourceconvertertest.cpp )

add_executable( exportmailjobinterfacetest ${exportmailjobinterfacetest_SRCS})
add_test(NAME exportmailjobinterfacetest COMMAND exportmailjobinterfacetest)
ecm_mark_as_test(exportmailjobinterfacetest)
target_link_libraries(exportmailjobinterfacetest Qt5::Test pimdataexporterprivate KF5::ConfigCore)
