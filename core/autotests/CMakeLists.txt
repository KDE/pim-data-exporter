####
set( utilstest_SRCS utilstest.cpp  )
add_executable( utilstest ${utilstest_SRCS})
add_test(NAME utilstest COMMAND utilstest)
ecm_mark_as_test(utilstest)
target_link_libraries( utilstest Qt5::Test pimdataexporterprivate KF5::ConfigCore)
#####
set( resourceconverterclasstest_SRCS resourceconverterclasstest.cpp resourceconvertertest.cpp )
add_executable( resourceconverterclasstest ${resourceconverterclasstest_SRCS})
add_test(NAME resourceconverterclasstest COMMAND resourceconverterclasstest)
ecm_mark_as_test(resourceconverterclasstest)
target_link_libraries( resourceconverterclasstest Qt5::Test KF5::ConfigCore pimdataexporterprivate)


add_subdirectory(notes)
add_subdirectory(calendar)
add_subdirectory(addressbook)
