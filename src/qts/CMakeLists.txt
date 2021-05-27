add_library(krossqtsplugin MODULE)

target_sources(krossqtsplugin PRIVATE
    plugin.cpp
)

ecm_qt_declare_logging_category(krossqtsplugin
    HEADER kross_qts_plugin_debug.h
    IDENTIFIER KROSS_QTS_PLUGIN_LOG
    CATEGORY_NAME kf.kross.qts_plugin
)

target_link_libraries(krossqtsplugin
    KF5::I18n
    KF5::KrossCore
    Qt5::UiTools
    Qt5::Script
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
)

install(TARGETS krossqtsplugin DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/script)
