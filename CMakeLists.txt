# Compulsory
cmake_minimum_required(VERSION 3.20)

# Define project here
project(hello_world
        VERSION      1.3
        DESCRIPTION  "Hello World"
        HOMEPAGE_URL "https://docs.wxwidgets.org/3.0/overview_helloworld.html"
        LANGUAGES    C CXX) # Enable C language for third party libraries

# Activate verbose mode
#set(CMAKE_VERBOSE_MAKEFILE ON)
# Debug find package instruction
#set(CMAKE_FIND_DEBUG_MODE TRUE)

# Detect presence of multi configuration generators
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

# Make Release version the default (only for single configuration generators)
if(NOT CMAKE_BUILD_TYPE AND NOT IS_MULTI_CONFIG)
  message(STATUS "Setting build type to 'Release' as none was specified")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Generation folder (into Release or Debug)
if (NOT IS_MULTI_CONFIG)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${CMAKE_BUILD_TYPE} OUTPUT_VARIABLE CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${CMAKE_BUILD_TYPE} OUTPUT_VARIABLE CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${CMAKE_BUILD_TYPE} OUTPUT_VARIABLE CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
endif()

# Add c++11 support whatever the compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# FINDING DEPENDENCIES
#############################################################

# Find wxWidgets
find_package(wxWidgets 3.0 REQUIRED core base adv)
include(${wxWidgets_USE_FILE})

# BUILDING
#############################################################

# Add executable target
add_executable(hello main.cpp)

# Link executable to wxWidgets library
target_link_libraries(hello PRIVATE ${wxWidgets_LIBRARIES})

# PACKAGING
#############################################################
cmake_path(APPEND CMAKE_CURRENT_SOURCE_DIR LICENSE OUTPUT_VARIABLE LICENSE_FILE_PATH)
cmake_path(APPEND CMAKE_CURRENT_SOURCE_DIR README.md OUTPUT_VARIABLE README_FILE_PATH)
# Package folder
cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${PROJECT_NAME} OUTPUT_VARIABLE PROJECT_FILE_PATH)
if (MSVC)
  # Archive file path
  set(ARCHIVE_FILE_NAME ${PROJECT_NAME}_${PROJECT_VERSION}_win64.zip)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR $<CONFIG> ${ARCHIVE_FILE_NAME} OUTPUT_VARIABLE ARCHIVE_FILE_PATH)

  # Need to copy C++ freeglut shared library to binary directory
  add_custom_target(package_gui
    COMMAND ${CMAKE_COMMAND} -E echo "Creating ${ARCHIVE_FILE_PATH}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_FILE_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:hello> ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LICENSE_FILE_PATH} ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${README_FILE_PATH}  ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E tar "c" ${ARCHIVE_FILE_PATH} --format=zip -- ${PROJECT_NAME}
  )
else()
  # Get Name/Version of the Linux
  # https://stackoverflow.com/a/41592428/3952924
  find_program(LSB_RELEASE_EXEC lsb_release)
  execute_process(COMMAND ${LSB_RELEASE_EXEC} -rs OUTPUT_VARIABLE LSB_OS_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND ${LSB_RELEASE_EXEC} -is OUTPUT_VARIABLE LSB_OS_NAME OUTPUT_STRIP_TRAILING_WHITESPACE)
  # Archive file path
  set(ARCHIVE_FILE_NAME ${PROJECT_NAME}_${PROJECT_VERSION}_${LSB_OS_NAME}_${LSB_OS_VERSION}.tar.gz)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR $<CONFIG> ${ARCHIVE_FILE_NAME} OUTPUT_VARIABLE ARCHIVE_FILE_PATH)
  
  add_custom_target(package_gui
    COMMAND ${CMAKE_COMMAND} -E echo "Creating ${ARCHIVE_FILE_PATH}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_FILE_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:hello> ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LICENSE_FILE_PATH} ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${README_FILE_PATH}  ${PROJECT_FILE_PATH}
    COMMAND ${CMAKE_COMMAND} -E tar "cz" ${ARCHIVE_FILE_PATH} -- ${PROJECT_NAME}
  )
endif()

# Compile executable before packaging
add_dependencies(package_gui hello)
