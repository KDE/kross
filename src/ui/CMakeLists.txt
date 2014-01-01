
set(krossui_LIB_SRCS
   actioncollectionmodel.cpp
   actioncollectionview.cpp
   scriptingplugin.cpp
#   guiclient.cpp
   )

add_library(KF5KrossUi ${krossui_LIB_SRCS})
generate_export_header(KF5KrossUi BASE_NAME KrossUi)

target_include_directories(KF5KrossUi INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/krossui>" "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KrossUi>")

target_link_libraries(KF5KrossUi
   KF5::KrossCore
   KF5::WidgetsAddons
   KF5::KIOWidgets
   KF5::Parts
   KF5::I18n
   KF5::IconThemes
)
set_target_properties(KF5KrossUi PROPERTIES VERSION ${KROSS_VERSION_STRING}
                                            SOVERSION ${KROSS_SOVERSION}
                                            EXPORT_NAME KrossUi
                     )

target_include_directories(KF5KrossUi PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

add_library(KF5::KrossUi ALIAS KF5KrossUi)

ecm_generate_headers(
  ActionCollectionModel
  ActionCollectionView
  ScriptingPlugin

  MODULE_NAME KrossUi/Kross
  REQUIRED_HEADERS KrossUi_HEADERS
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KrossUi/Kross DESTINATION ${INCLUDE_INSTALL_DIR}/KrossUi COMPONENT Devel)

install(TARGETS KF5KrossUi EXPORT KF5KrossTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krossui_export.h
  plugin.h # compat header
  model.h # compat header
  view.h # compat header
  ${KrossUi_HEADERS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/krossui/kross/ui COMPONENT Devel)
