include(ECMMarkAsTest)

set(krossqtstest_SRCS main.cpp)
add_executable(krossqtstest qts.cpp)
ecm_mark_as_test(krossqtstest)
target_link_libraries(krossqtstest
   KF5::KrossCore
   Qt5::Script
   KF5::I18n
   Qt5::Widgets
)

add_test(NAME krossqtstest-test-es COMMAND krossqtstest ${CMAKE_CURRENT_SOURCE_DIR}/qtstest.es)

#set(krosstest_SRCS
#   testobject.cpp
#   main.cpp )


#add_executable(krosstest ${krosstest_SRCS})
#target_link_libraries(krosstest KF5::KrossCore KF5::CoreAddons KF5::ConfigCore Qt5::Script Qt5::Widgets Qt5::Xml)

#add_test(NAME kross-guiform-py COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.py)
#add_test(NAME kross-guiform-rb COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.rb)
#add_test(NAME kross-guiqt-py COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiqt.py)
#add_test(NAME kross-guiqt-rb COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiqt.rb)
#add_test(NAME kross-guitk-py COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguitk.py)
#add_test(NAME kross-test-py COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testkross.py)
#add_test(NAME kross-unittest-es COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.es)
#add_test(NAME kross-unittest-py COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.py)
#add_test(NAME kross-unittest-rb COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/unittest.rb)

# UI files have no interpreter
# add_test(NAME kross-guiform-ui COMMAND krosstest ${CMAKE_CURRENT_SOURCE_DIR}/testguiform.ui)
