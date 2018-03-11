set(krossqts_PART_SRCS interpreter.cpp script.cpp)
ecm_qt_declare_logging_category(
    krossqts_PART_SRCS
    HEADER kross_qtscript_debug.h
    IDENTIFIER KROSS_QTSCRIPT_LOG
    CATEGORY_NAME org.kde.kross.qtscript
)
add_library(krossqts MODULE ${krossqts_PART_SRCS})
target_link_libraries(krossqts
    KF5::KrossCore
    Qt5::Script
    Qt5::Core
    Qt5::Widgets
)

install(TARGETS krossqts DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
