
file(GLOB _KROSSUIHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
#message(STATUS _KROSSUIHEADERS=${_KROSSUIHEADERS})
foreach(_KROSSUIHEADER ${_KROSSUIHEADERS})
   string(REGEX MATCH ".*_p\\.h" _ISPRIVATE ${_KROSSUIHEADER})
   #message(STATUS "_ISPRIVATE=${_ISPRIVATE} _KROSSUIHEADER=${_KROSSUIHEADER}")
    if (NOT _ISPRIVATE)
       if (NOT EXISTS ${_INCDIR}/ui/${_KROSSUIHEADER})
          file(WRITE ${_INCDIR}/ui/${_KROSSUIHEADER} "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${_KROSSUIHEADER}\"\n")
        endif()
    endif()
endforeach()
if (NOT EXISTS ${_INCDIR}/ui/krossui_export.h)
    file(WRITE ${_INCDIR}/ui/krossui_export.h "#include \"${CMAKE_CURRENT_BINARY_DIR}/krossui_export.h\"\n")
endif()

add_library(KF5KrossUi)
add_library(KF5::KrossUi ALIAS KF5KrossUi)

target_sources(KF5KrossUi PRIVATE
   actioncollectionmodel.cpp
   actioncollectionview.cpp
   scriptingplugin.cpp
#   guiclient.cpp
   )
ecm_qt_declare_logging_category(KF5KrossUi
    HEADER krossui_debug.h
    IDENTIFIER KROSS_UI_LOG
    CATEGORY_NAME kf.kross.ui
)

generate_export_header(KF5KrossUi BASE_NAME KrossUi)

target_include_directories(KF5KrossUi INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KrossUi>")

target_link_libraries(KF5KrossUi
PUBLIC
   KF5::KrossCore
   KF5::Parts   # KParts::Plugin in public API
PRIVATE
   KF5::I18n
   KF5::IconThemes # KIconButton
)
set_target_properties(KF5KrossUi PROPERTIES VERSION ${KROSS_VERSION}
                                            SOVERSION ${KROSS_SOVERSION}
                                            EXPORT_NAME KrossUi
                     )
target_include_directories(KF5KrossUi PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>")

ecm_generate_headers(KrossUi_CamelCase_HEADERS
  HEADER_NAMES
  ActionCollectionModel
  ActionCollectionView
  ScriptingPlugin

  PREFIX Kross/Ui
  REQUIRED_HEADERS KrossUi_HEADERS
)
install(FILES ${KrossUi_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossUi/Kross/Ui COMPONENT Devel)

install(TARGETS KF5KrossUi EXPORT KF5KrossTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/krossui_export.h
  plugin.h # compat header
  model.h # compat header
  view.h # compat header
  ${KrossUi_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossUi/kross/ui COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KrossUi LIB_NAME KF5KrossUi DEPS "KrossCore KParts" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KrossUi)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
