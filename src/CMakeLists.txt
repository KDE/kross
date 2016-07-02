set(_INCDIR ${CMAKE_CURRENT_BINARY_DIR}/include/kross)
file(MAKE_DIRECTORY ${_INCDIR})

add_subdirectory( core )
add_subdirectory( ui )
add_subdirectory( modules )
add_subdirectory( console )

# QtScript backend
add_subdirectory( qts )
