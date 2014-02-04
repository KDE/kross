include(ECMMarkAsTest)

set(krossqtstest_SRCS main.cpp)
add_executable(krossqtstest qts.cpp)
ecm_mark_as_test(krossqtstest)
target_link_libraries(krossqtstest
   krossqtsplugin
   KF5::KrossCore
   Qt5::Script
   KF5::I18n
   Qt5::Widgets
)

add_test(krossqtstest-test-es krossqtstest ${CMAKE_CURRENT_SOURCE_DIR}/qtstest.es)

#set(krosstest_SRCS
#   testobject.cpp
#   main.cpp )


#add_executable(krosstest ${krosstest_SRCS})
#target_link_libraries(krosstest KF5::KrossCore KF5::CoreAddons KF5::ConfigCore Qt5::Script Qt5::Widgets Qt5::Xml)

#add_test(kross-guiform-py krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.py)
#add_test(kross-guiform-rb krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.rb)
#add_test(kross-guiqt-py krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiqt.py)
#add_test(kross-guiqt-rb krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiqt.rb)
#add_test(kross-guitk-py krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguitk.py)
#add_test(kross-test-py krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testkross.py)
#add_test(kross-unittest-es krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.es)
#add_test(kross-unittest-py krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.py)
#add_test(kross-unittest-rb krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.rb)

# UI files have no interpreter
# add_test(kross-guiform-ui krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.ui)
