add_library(krossqts MODULE)

target_sources(krossqts PRIVATE
    interpreter.cpp
    script.cpp
)

ecm_qt_declare_logging_category(krossqts
    HEADER kross_qtscript_debug.h
    IDENTIFIER KROSS_QTSCRIPT_LOG
    CATEGORY_NAME kf.kross.qtscript
)

target_link_libraries(krossqts
    KF5::KrossCore
    Qt5::Script
    Qt5::Core
    Qt5::Widgets
)

install(TARGETS krossqts DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
