cmake_minimum_required(VERSION 3.13)

# Set project name
project(SimpleUnitTestLibrary LANGUAGES CXX)

add_library(SimpleUnitTestLibrary INTERFACE)

# Set include directories for the library target (this will propagate to consumers)
target_include_directories(SimpleUnitTestLibrary INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleUnitTestLibrary/Headers
)

# Alias for easier linking
add_library(SUTL ALIAS SimpleUnitTestLibrary)

# Set C++ standard for the library
set_target_properties(SimpleUnitTestLibrary PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
