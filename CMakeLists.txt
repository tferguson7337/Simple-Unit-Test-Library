cmake_minimum_required(VERSION 3.13)

# Set project name
project(SUTL LANGUAGES CXX)

# Set root directory of code
set(PROJ_CODE_ROOT ${SUTL_SOURCE_DIR}/SimpleUnitTestLibrary)

# Location of .h files
include_directories(${PROJ_CODE_ROOT}/Headers)

# Location of .cpp files
file(GLOB SOURCES "${PROJ_CODE_ROOT}/Source/*.cpp")

# Add library target to be built from source.
add_library(SUTL STATIC ${SOURCES})

# Build using C++17, Release configuration.
set_target_properties(SUTL PROPERTIES CXX_STANDARD 17)
set_target_properties(SUTL PROPERTIES BUILD_TYPE Release)


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Compile with all+extra warnings, error on warnings.
    set(GNU_WARNING_FLAGS "-Wall -Wextra -Werror -pedantic")
    message("Adding GNU warning flags[${GNU_WARNING_FLAGS}]")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GNU_WARNING_FLAGS}")
endif()


# Remove any default prefix from output (e.g., "lib").
set_target_properties(SUTL PROPERTIES PREFIX "")

install(TARGETS SUTL DESTINATION ${SUTL_SOURCE_DIR}/build)