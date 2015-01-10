if( Qt5UiTools_FOUND )
	############################################
	# the forms module

	set(krossmoduleforms_SRCS form.cpp)
    add_library(KrossModuleForms MODULE ${krossmoduleforms_SRCS})

    target_link_libraries(KrossModuleForms
       Qt5::UiTools
       KF5::Parts
       KF5::KIOFileWidgets
       KF5::KrossCore
       KF5::KrossUi
       KF5::I18n
    )
    install(TARGETS KrossModuleForms DESTINATION ${KDE_INSTALL_PLUGINDIR})
endif()

############################################
# the kdetranslation module

set(krossmodulekdetranslation_SRCS translation.cpp)
add_library(KrossModuleKdeTranslation MODULE ${krossmodulekdetranslation_SRCS})

target_link_libraries(KrossModuleKdeTranslation
   KF5::Parts
   KF5::KrossCore
   KF5::I18n
)
install(TARGETS KrossModuleKdeTranslation DESTINATION ${KDE_INSTALL_PLUGINDIR})
