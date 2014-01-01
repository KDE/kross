
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

target_include_directories(KF5KrossCore INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/krosscore>" "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KrossCore>")

target_link_libraries(KF5KrossCore PRIVATE Qt5::Script Qt5::Xml Qt5::Widgets KF5::ConfigCore KF5::I18n)
target_link_libraries(KF5KrossCore PUBLIC Qt5::Script Qt5::Xml)

target_include_directories(KF5KrossCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

set_target_properties(KF5KrossCore PROPERTIES VERSION ${KROSS_VERSION_STRING}
                                   SOVERSION ${KROSS_SOVERSION}
                                   EXPORT_NAME KrossCore
                     )


ecm_generate_headers(
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

  MODULE_NAME KrossCore/Kross
  REQUIRED_HEADERS KrossCore_HEADERS
)
# apps include <Kross/Object>
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KrossCore/Kross DESTINATION ${INCLUDE_INSTALL_DIR}/KrossCore COMPONENT Devel)

install(TARGETS KF5KrossCore EXPORT KF5KrossLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

add_library(KF5::KrossCore ALIAS KF5KrossCore)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krosscore_export.h
  krossconfig.h
  ${KrossCore_HEADERS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/krosscore/kross/core COMPONENT Devel) # apps include <kross/core/object.h>

install(TARGETS KF5KrossCore EXPORT KF5KrossTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
