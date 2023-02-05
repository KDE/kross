cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.103.0") # handled by release scripts
set(KF_DEP_VERSION "5.103.0") # handled by release scripts
project(Kross VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.103.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})


include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMGenerateExportHeader)
include(ECMDeprecationSettings)
include(ECMQtDeclareLoggingCategory)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

ecm_setup_version(PROJECT VARIABLE_PREFIX KROSS
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kross_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KrossConfigVersion.cmake"
                        SOVERSION 5)

set(REQUIRED_QT_VERSION 5.15.2)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Script Xml Widgets UiTools)


find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)
set_package_properties(Qt5Test PROPERTIES
   PURPOSE "Required for tests"
   TYPE OPTIONAL
   )
add_feature_info("Qt5Test" Qt5Test_FOUND "Required for building tests")
if (NOT Qt5Test_FOUND)
   set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree.")
endif()

find_package(KF5Completion ${KF_DEP_VERSION} REQUIRED)
find_package(KF5CoreAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5I18n ${KF_DEP_VERSION} REQUIRED)
find_package(KF5IconThemes ${KF_DEP_VERSION} REQUIRED)
find_package(KF5KIO ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Parts ${KF_DEP_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5XmlGui ${KF_DEP_VERSION} REQUIRED)
find_package(KF5DocTools ${KF_DEP_VERSION})

# remove definitions set by KDEFrameworkCompilerSettings which we fail to meet
remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_FOREACH)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
# ideally we would support QT_NO_KEYWORDS, but at least we can do this
add_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
)
add_definitions(-DTRANSLATION_DOMAIN=\"kross5\")
ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
if (KF5DocTools_FOUND)
    add_subdirectory(docs)
endif()

add_subdirectory(src)
if (BUILD_TESTING)
add_subdirectory(autotests)
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Kross")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5KrossConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5KrossConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kross_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5KrossConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5KrossConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5KrossTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5KrossTargets.cmake NAMESPACE KF5::)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
