######################################################################
# Quicky - a simple Wiki-style note-taking application.
#
# Qt 5 build. This project started life as a Qt 4 / qmake project; the
# .pro file has been replaced by this CMake project.
######################################################################

cmake_minimum_required(VERSION 3.16)

project(quicky
    VERSION 0.4
    DESCRIPTION "Simple Wiki-style note-taking application"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Run moc, uic and rcc automatically.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Default to a release build, as the original qmake project did.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# On Haiku, Qt 5 is installed in the system package tree. Also look in the
# per-user non-packaged tree, which is where a hand-built Qt 5 would be.
if(CMAKE_SYSTEM_NAME STREQUAL "Haiku")
    list(APPEND CMAKE_PREFIX_PATH
        /boot/system
        "$ENV{HOME}/config/non-packaged")
endif()

# QProcess::splitCommand() needs Qt 5.15; Haiku ships 5.15.x.
find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)

set(QUICKY_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/wikiview.cpp
    src/wikiwidget.cpp
    src/wikieditor.cpp
    src/finddialog.cpp
    src/stylesheetdialog.cpp)

set(QUICKY_HEADERS
    src/mainwindow.h
    src/wikiview.h
    src/wikiwidget.h
    src/wikieditor.h
    src/finddialog.h
    src/stylesheetdialog.h
    src/version.h)

set(QUICKY_FORMS
    src/wikiwidget.ui
    src/finddialog.ui
    src/stylesheetdialog.ui)

add_executable(quicky
    ${QUICKY_SOURCES}
    ${QUICKY_HEADERS}
    ${QUICKY_FORMS}
    src/quicky.qrc)

# uic emits #include <wikiview.h> / <wikieditor.h> for the custom widgets
# used in wikiwidget.ui, so the source directory must be on the include path.
target_include_directories(quicky PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(quicky PRIVATE Qt5::Widgets)

# The original qmake project built with CONFIG += warn_all.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(quicky PRIVATE -Wall)
endif()

# qDebug() calls are compiled out of anything but a debug build, which is
# what the old qmake project did with QT_NO_DEBUG_OUTPUT.
target_compile_definitions(quicky PRIVATE
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>)
