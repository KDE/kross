############################################
# the forms module

set(krossmoduleforms_SRCS form.cpp)
add_library(krossmoduleforms MODULE ${krossmoduleforms_SRCS})

target_link_libraries(krossmoduleforms
    Qt5::UiTools
    KF5::Parts
    KF5::KIOFileWidgets
    KF5::KrossCore
    KF5::KrossUi
    KF5::I18n
    )
install(TARGETS krossmoduleforms DESTINATION ${KDE_INSTALL_QTPLUGINDIR})

############################################
# the kdetranslation module

set(krossmodulekdetranslation_SRCS translation.cpp)
add_library(krossmodulekdetranslation MODULE ${krossmodulekdetranslation_SRCS})

target_link_libraries(krossmodulekdetranslation
   KF5::Parts
   KF5::KrossCore
   KF5::I18n
)
install(TARGETS krossmodulekdetranslation DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
