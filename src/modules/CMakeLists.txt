############################################
# the forms module

add_library(krossmoduleforms MODULE)

target_sources(krossmoduleforms PRIVATE
    form.cpp
)

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

add_library(krossmodulekdetranslation MODULE)

target_sources(krossmodulekdetranslation PRIVATE
    translation.cpp
)

target_link_libraries(krossmodulekdetranslation
   KF5::Parts
   KF5::KrossCore
   KF5::I18n
)
install(TARGETS krossmodulekdetranslation DESTINATION ${KDE_INSTALL_QTPLUGINDIR})
