
file(GLOB _KROSSCOREHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
#message(STATUS _KROSSCOREHEADERS=${_KROSSCOREHEADERS})
foreach(_KROSSCOREHEADER ${_KROSSCOREHEADERS})
    string(REGEX MATCH ".*_p\\.h" _ISPRIVATE ${_KROSSCOREHEADER})
    #message(STATUS "_ISPRIVATE=${_ISPRIVATE} _KROSSCOREHEADER=${_KROSSCOREHEADER}")
    if (NOT _ISPRIVATE)
        if (NOT EXISTS ${_INCDIR}/core/${_KROSSCOREHEADER})
            file(WRITE ${_INCDIR}/core/${_KROSSCOREHEADER} "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${_KROSSCOREHEADER}\"\n")
        endif()
    endif()
endforeach()
if (NOT EXISTS ${_INCDIR}/core/krosscore_export.h)
    file(WRITE ${_INCDIR}/core/krosscore_export.h "#include \"${CMAKE_CURRENT_BINARY_DIR}/krosscore_export.h\"\n")
endif()

add_library(KF5KrossCore)
add_library(KF5::KrossCore ALIAS KF5KrossCore)

target_sources(KF5KrossCore PRIVATE
   krossconfig.cpp
   interpreter.cpp
   script.cpp
   action.cpp
   actioncollection.cpp
   manager.cpp
   metafunction.cpp
   object.cpp
)

ecm_qt_declare_logging_category(KF5KrossCore
    HEADER kross_debug.h
    IDENTIFIER KROSS_LOG
    CATEGORY_NAME kf.kross.core
)

generate_export_header(KF5KrossCore BASE_NAME KrossCore)

target_include_directories(KF5KrossCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KrossCore>")

target_link_libraries(KF5KrossCore
PUBLIC
   Qt5::Script
   Qt5::Xml
PRIVATE
   Qt5::Widgets
   KF5::I18n
)

target_include_directories(KF5KrossCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

set_target_properties(KF5KrossCore PROPERTIES VERSION ${KROSS_VERSION}
                                   SOVERSION ${KROSS_SOVERSION}
                                   EXPORT_NAME KrossCore
                     )


ecm_generate_headers(KrossCore_CamelCase_HEADERS
  HEADER_NAMES
  ErrorInterface
  ChildrenInterface
  WrapperInterface
  Interpreter
  Script
  Action
  ActionCollection
  Manager
  Object
  MetaType
  MetaFunction
  KrossConfig

  PREFIX Kross/Core
  REQUIRED_HEADERS KrossCore_HEADERS
)
# apps include <Kross/Core/Object> or <kross/core/object.h>
install(FILES ${KrossCore_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossCore/Kross/Core COMPONENT Devel)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krosscore_export.h
  ${KrossCore_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossCore/kross/core COMPONENT Devel) # apps include <kross/core/object.h>

install(TARGETS KF5KrossCore EXPORT KF5KrossTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KrossCore LIB_NAME KF5KrossCore DEPS "script xml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossCore)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
