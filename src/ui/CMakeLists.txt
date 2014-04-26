
set(krossui_LIB_SRCS
   actioncollectionmodel.cpp
   actioncollectionview.cpp
   scriptingplugin.cpp
#   guiclient.cpp
   )

add_library(KF5KrossUi ${krossui_LIB_SRCS})
generate_export_header(KF5KrossUi BASE_NAME KrossUi)

target_include_directories(KF5KrossUi INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KrossUi>")

target_link_libraries(KF5KrossUi
PUBLIC
   KF5::KrossCore
   KF5::Parts   # KParts::Plugin in public API
PRIVATE
   KF5::I18n
   KF5::IconThemes # KIconButton
)
set_target_properties(KF5KrossUi PROPERTIES VERSION ${KROSS_VERSION_STRING}
                                            SOVERSION ${KROSS_SOVERSION}
                                            EXPORT_NAME KrossUi
                     )

target_include_directories(KF5KrossUi PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

add_library(KF5::KrossUi ALIAS KF5KrossUi)

ecm_generate_headers(KrossUi_CamelCase_HEADERS
  HEADER_NAMES
  ActionCollectionModel
  ActionCollectionView
  ScriptingPlugin

  PREFIX Kross/Ui
  REQUIRED_HEADERS KrossUi_HEADERS
)
install(FILES ${KrossUi_CamelCase_HEADERS} DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KrossUi/Kross/Ui COMPONENT Devel)

install(TARGETS KF5KrossUi EXPORT KF5KrossTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krossui_export.h
  plugin.h # compat header
  model.h # compat header
  view.h # compat header
  ${KrossUi_HEADERS}
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KrossUi/kross/ui COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KrossUi LIB_NAME KF5KrossUi DEPS "KrossCore KParts" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
