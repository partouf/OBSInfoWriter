cmake_minimum_required(VERSION 3.24)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(OBSInfoWriter VERSION 1.0.0)

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

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
FetchContent_Declare(
  Crosscables
  OVERRIDE_FIND_PACKAGE
  GIT_REPOSITORY https://github.com/partouf/Crosscables.git
  GIT_TAG master)

# Add your custom source files here - header files are optional and only required for visibility e.g. in Xcode or Visual
# Studio
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE OutputFormat/OutputFormat.SRT.cpp
          OutputFormat/OutputFormat.CSV.cpp
          OutputFormat/OutputFormat.EDL.cpp
          OutputFormat/OutputFormat.Default.cpp
          InfoWriter.cpp
          InfoWriterSettings.cpp
          OBSStudioInfoWriter.cpp)

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
