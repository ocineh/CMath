cmake_minimum_required(VERSION 3.21)
project(projet VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_subdirectory(docs)
else()
    message(STATUS "Doxygen not found, not building docs")
endif()

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(app)

# The test are here
include(CTest)
add_subdirectory(test)