include(ECMMarkAsTest)

## These unit tests must move to kross-interpreters.git, they require the python kross interpreter!
if (FALSE)
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
endif()

add_executable(metafunctiontest metafunctiontest.cpp)
ecm_mark_as_test(metafunctiontest)
target_link_libraries(metafunctiontest
   Qt5::Test
   KF5::KrossCore
   )
add_test(NAME kross-metafunctiontest COMMAND metafunctiontest)
