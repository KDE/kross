########### next target ###############
set(krossqtsplugin_LIB_SRCS plugin.cpp)
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
add_library(krossqts MODULE ${krossqts_PART_SRCS})
target_link_libraries(krossqts
   KF5::I18n
   KF5::KrossCore
   Qt5::Script
   Qt5::Widgets
   Qt5::Gui
)

install(TARGETS krossqts DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
