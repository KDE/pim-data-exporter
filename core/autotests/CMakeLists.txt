set( utilstest_SRCS utilstest.cpp  )

add_executable( utilstest ${utilstest_SRCS})
add_test(NAME utilstest COMMAND utilstest)
ecm_mark_as_test(utilstest)
target_link_libraries( utilstest Qt5::Test pimdataexporterprivate KF5::ConfigCore)

add_subdirectory(notes)
add_subdirectory(calendar)
