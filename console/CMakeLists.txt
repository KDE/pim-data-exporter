
include_directories(${AKONADI_INCLUDE_DIR} 
    ${pim-data-exporter_SOURCE_DIR}/core/
    )
add_executable(pimdataexporterconsole)

target_sources(pimdataexporterconsole PRIVATE
    importexportprogressindicatorconsole.cpp
    pimdataexporterconsole.cpp
    loginfo.cpp
    loginfile.cpp
    main.cpp
    importexportprogressindicatorconsole.h
    pimdataexporterconsole.h
    loginfo.h
    loginfile.h
    )


ecm_qt_declare_logging_category(pimdataexporterconsole HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole
        DESCRIPTION "pimdataexporter (pimdataexporter console)"
        OLD_CATEGORY_NAMES log_pimsettingexporterconsole org.kde.pim.pimsettingexporterconsole
        EXPORT PIMDATAEXPORTER
    )


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(pimdataexporterconsole PROPERTIES UNITY_BUILD ON)
endif()

target_link_libraries(pimdataexporterconsole KF5::ConfigCore KF5::CoreAddons KF5::I18n KF5::Archive KF5::AkonadiCore   KF5::MailCommon
    pimdataexporterprivate
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
install(TARGETS pimdataexporterconsole ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

