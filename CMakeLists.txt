cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(OBSInfoWriter VERSION 2.7.4)

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)
option(DISABLE_JUMPROPES "Disable Jumpropes (networking) library (not actually used)" ON)
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
  enable_testing()
endif()

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

# Replace `com.example.obs-plugin-template` with a unique Bundle ID for macOS releases (used both in the installer and
# when submitting the installer for notarization)
set(MACOS_BUNDLEID "com.example.${CMAKE_PROJECT_NAME}")

# Replace `me@contoso.com` with the maintainer email address you want to put in Linux packages
set(LINUX_MAINTAINER_EMAIL "partouf@gmail.com")

# Download dependencies
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  Crosscables
  OVERRIDE_FIND_PACKAGE
  GIT_REPOSITORY https://github.com/partouf/Crosscables.git
  GIT_TAG master
  EXCLUDE_FROM_ALL
)

# Add your custom source files here - header files are optional and only required for visibility e.g. in Xcode or Visual
# Studio
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
    OutputFormat/OutputFormat.SRT.cpp
    OutputFormat/OutputFormat.CSV.cpp
    OutputFormat/OutputFormat.EDL.cpp
    OutputFormat/OutputFormat.Default.cpp
    InfoWriter.cpp
    InfoWriterSettings.cpp
    InfoWriterObsUtils.cpp
    OBSStudioInfoWriter.cpp
)

# Import libobs as main plugin dependency
find_package(libobs REQUIRED)

find_package(obs-frontend-api REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/crosscables-src)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Groundfloor)
find_package(Crosscables REQUIRED)

# --- Platform-independent build settings ---
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)
# --- End of section ---

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})

# add_executable(testconsole)
# target_sources(
#   testconsole
#   PRIVATE
#     OutputFormat/OutputFormat.SRT.cpp
#     OutputFormat/OutputFormat.CSV.cpp
#     OutputFormat/OutputFormat.EDL.cpp
#     OutputFormat/OutputFormat.Default.cpp
#     InfoWriter.cpp
#     InfoWriterSettings.cpp
#     DummyUtils.cpp
#     main.cpp
# )
# target_compile_features(testconsole PRIVATE cxx_std_17)

# target_link_libraries(testconsole PRIVATE Groundfloor)

if(BUILD_TESTS)
  add_executable(
    test_infowriter
    tests/test_filename_format.cpp
    OutputFormat/OutputFormat.SRT.cpp
    OutputFormat/OutputFormat.CSV.cpp
    OutputFormat/OutputFormat.EDL.cpp
    OutputFormat/OutputFormat.Default.cpp
    InfoWriter.cpp
    InfoWriterSettings.cpp
    DummyUtils.cpp
    DummyPlatform.cpp
  )
  target_compile_features(test_infowriter PRIVATE cxx_std_17)
  target_include_directories(test_infowriter PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/crosscables-src ${CMAKE_SOURCE_DIR}/tests/stubs)
  FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.5.0)
  FetchContent_MakeAvailable(Catch2)

  target_link_libraries(test_infowriter PRIVATE Groundfloor Catch2::Catch2WithMain)

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)
  catch_discover_tests(test_infowriter)
endif()
