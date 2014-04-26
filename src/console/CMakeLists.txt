#This binary is used for any non-GUI Kross scripts, e.g. Calligra Sheets has some
add_executable(kf5kross main.cpp)
target_link_libraries(kf5kross
   KF5::CoreAddons
   KF5::KrossCore
   KF5::I18n
   Qt5::Widgets
)

install(TARGETS kf5kross ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
