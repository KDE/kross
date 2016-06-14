########### next target ###############
set(krossqtsplugin_LIB_SRCS plugin.cpp)
ecm_qt_declare_logging_category(
    krossqtsplugin_LIB_SRCS
    HEADER kross_qts_plugin_debug.h
    IDENTIFIER KROSS_QTS_PLUGIN_LOG
    CATEGORY_NAME org.kde.kross.qts_plugin
)
add_library(krossqtsplugin MODULE ${krossqtsplugin_LIB_SRCS})
target_link_libraries(krossqtsplugin
   Qt5::UiTools
   KF5::I18n
   KF5::KrossCore
   Qt5::Script
   Qt5::Widgets
   Qt5::Gui
)

install(TARGETS krossqtsplugin DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/script)

########### next target ###############

set(krossqts_PART_SRCS interpreter.cpp script.cpp)
ecm_qt_declare_logging_category(
    krossqts_PART_SRCS
    HEADER kross_qtscript_debug.h
    IDENTIFIER KROSS_QTSCRIPT_LOG
    CATEGORY_NAME org.kde.kross.qtscript
)
add_library(krossqts MODULE ${krossqts_PART_SRCS})
target_link_libraries(krossqts
   KF5::I18n
   KF5::KrossCore
   Qt5::Script
   Qt5::Widgets
   Qt5::Gui
)

install(TARGETS krossqts DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
