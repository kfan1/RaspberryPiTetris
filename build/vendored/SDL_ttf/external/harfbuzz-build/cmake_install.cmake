# Install script for directory: /home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb.h"
    "/home/kevinlifan/SWENG452W/RaspberryPiTetris/vendored/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    )
endif()

