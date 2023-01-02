# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAINSTALL_DIR="${CMAKE_CURRENT_BINARY_DIR}/install" )

####
add_executable( utilstest utilstest.cpp utilstest.h)
add_test(NAME utilstest COMMAND utilstest)
ecm_mark_as_test(utilstest)
target_link_libraries( utilstest Qt::Test pimdataexporterprivate KF5::ConfigCore)

#####
add_library(test_resourceconverter STATIC
    resourceconvertertest.cpp
    generatelistfilefromarchive.cpp
    loadlistfromfile.cpp
    compareexportfile.cpp
    compareimportfile.cpp
    testexportfile.cpp
    testimportfile.cpp
    logcreatingressource.cpp
    comparefilehelper.cpp
    exportimportutil.cpp
    saveresourceconfigtest.cpp
    compareloggingfile.cpp
    testbackupresourcefilejob.cpp
    importexportprogressindicatortest.cpp
    resourceconvertertest.h
    generatelistfilefromarchive.h
    loadlistfromfile.h
    compareexportfile.h
    compareimportfile.h
    testexportfile.h
    testimportfile.h
    logcreatingressource.h
    comparefilehelper.h
    exportimportutil.h
    saveresourceconfigtest.h
    compareloggingfile.h
    testbackupresourcefilejob.h
    importexportprogressindicatortest.h
)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(test_resourceconverter PROPERTIES UNITY_BUILD ON)
endif()
target_link_libraries(test_resourceconverter PUBLIC KF5::ConfigCore pimdataexporterprivate KF5::Archive Qt::Test  KF5::AkonadiMime KF5::AkonadiCore KF5::WidgetsAddons)


#####
add_executable( resourceconverterclasstest resourceconverterclasstest.cpp resourceconverterclasstest.h)
add_test(NAME resourceconverterclasstest COMMAND resourceconverterclasstest)
ecm_mark_as_test(resourceconverterclasstest)
target_link_libraries( resourceconverterclasstest Qt::Test KF5::ConfigCore test_resourceconverter)

#####
add_executable( backupresourcefilejobimpltest backupresourcefilejobimpltest.cpp backupresourcefilejobimpltest.h)
add_test(NAME backupresourcefilejobimpltest COMMAND backupresourcefilejobimpltest)
ecm_mark_as_test(backupresourcefilejobimpltest)
target_link_libraries( backupresourcefilejobimpltest Qt::Test KF5::ConfigCore pimdataexporterprivate KF5::Archive)
###

add_subdirectory(notes)
add_subdirectory(calendar)
add_subdirectory(addressbook)
add_subdirectory(alarm)
add_subdirectory(akregator)
add_subdirectory(mail)
