cmake_minimum_required(VERSION 3.16...3.21)

project(OBSInfoWriter VERSION 1.0.0)
add_library(${CMAKE_PROJECT_NAME} MODULE)

# Replace `Your Name Here` with the name (yours or your organization's) you want to see as the
# author of the plugin (in the plugin's metadata itself and in the installers)
set(PLUGIN_AUTHOR "Patrick Quist")

# Replace `com.example.obs-plugin-template` with a unique Bundle ID for macOS releases (used both in
# the installer and when submitting the installer for notarization)
set(MACOS_BUNDLEID "com.example.${CMAKE_PROJECT_NAME}")

# Replace `me@contoso.com` with the maintainer email address you want to put in Linux packages
set(LINUX_MAINTAINER_EMAIL "partouf@gmail.com")

# Add your custom source files here - header files are optional and only required for visibility
# e.g. in Xcode or Visual Studio
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  OutputFormat/OutputFormat.SRT.cpp
  OutputFormat/OutputFormat.CSV.h
  OutputFormat/OutputFormat.EDL.cpp
  OutputFormat/OutputFormat.Default.cpp
  InfoWriter.cpp
  InfoWriterSettings.cpp
  OBSStudioInfoWriter.cpp)

# Import libobs as main plugin dependency
find_package(libobs REQUIRED)
include(cmake/ObsPluginHelpers.cmake)

find_package(obs-frontend-api REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)

include(FetchContent)
FetchContent_Declare(
  Crosscables
  GIT_REPOSITORY https://github.com/partouf/Crosscables.git
  GIT_TAG noexternals
)

FetchContent_MakeAvailable(Crosscables)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE rt)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/crosscables-src/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE libGroundfloor)

# /!\ TAKE NOTE: No need to edit things past this point /!\

# --- Platform-independent build settings ---
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

# --- End of section ---

# --- Windows-specific build settings and tasks ---
if(OS_WINDOWS)
  configure_file(cmake/bundle/windows/installer-Windows.iss.in
    ${CMAKE_BINARY_DIR}/installer-Windows.generated.iss)

  configure_file(cmake/bundle/windows/resource.rc.in ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.rc)
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.rc)

  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4)
  endif()

# --- End of section ---

# -- macOS specific build settings and tasks --
elseif(OS_MACOS)
  configure_file(cmake/bundle/macos/installer-macos.pkgproj.in
    ${CMAKE_BINARY_DIR}/installer-macos.generated.pkgproj)

  set(MACOSX_PLUGIN_GUI_IDENTIFIER "${MACOS_BUNDLEID}")
  set(MACOSX_PLUGIN_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}")
  set(MACOSX_PLUGIN_SHORT_VERSION_STRING "1")

  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall)

# --- End of section ---

# --- Linux-specific build settings and tasks ---
else()
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wno-unused-parameter)
endif()

# --- End of section ---
setup_plugin_target(${CMAKE_PROJECT_NAME})
