
set(krosscore_LIB_SRCS
   krossconfig.cpp
   interpreter.cpp
   script.cpp
   action.cpp
   actioncollection.cpp
   manager.cpp
   object.cpp
)

add_library(KF5KrossCore ${krosscore_LIB_SRCS})
generate_export_header(KF5KrossCore BASE_NAME KrossCore)

target_include_directories(KF5KrossCore INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KrossCore>")

target_link_libraries(KF5KrossCore
PUBLIC
   Qt5::Script
   Qt5::Xml
PRIVATE
   Qt5::Widgets
   KF5::ConfigCore
   KF5::I18n
)

target_include_directories(KF5KrossCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

set_target_properties(KF5KrossCore PROPERTIES VERSION ${KROSS_VERSION_STRING}
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

  PREFIX Kross/Core
  REQUIRED_HEADERS KrossCore_HEADERS
)
# apps include <Kross/Core/Object> or <kross/core/object.h>
install(FILES ${KrossCore_CamelCase_HEADERS} DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KrossCore/Kross/Core COMPONENT Devel)

add_library(KF5::KrossCore ALIAS KF5KrossCore)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krosscore_export.h
  krossconfig.h
  ${KrossCore_HEADERS}
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KrossCore/kross/core COMPONENT Devel) # apps include <kross/core/object.h>

install(TARGETS KF5KrossCore EXPORT KF5KrossTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KrossCore LIB_NAME KF5KrossCore DEPS "script xml" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
