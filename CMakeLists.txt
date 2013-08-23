# Copyright 2013 Douglas Linder
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required (VERSION 2.8)
project (ASDL)
set(BIN "test")

# Debug mode?
set(CMAKE_BUILD_TYPE Release)

# We have configure style deps, sadly.
include(ExternalProject)

# Flags
set(CMAKE_C_FLAGS "-Wall -pedantic -Wimplicit-function-declaration -Wreturn-type -Wstrict-prototypes -Wextra -fPIC")
if(NOT WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif()

# Code type
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Archive names
set(SDL "SDL-2.0.0-7635")
set(SDL_IMG "SDL2_image-2.0.0")
set(SDL_MIXER "SDL2_mixer-2.0.0")
set(SDL_TTF "SDL2_ttf-2.0.12")
set(SDL_GFX "sdl2gfx-code")
set(FREETYPE "freetype-2.5.0.1")

# Decompress archives 
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/src)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${SDL}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${SDL_TTF}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${SDL_IMG}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${SDL_GFX}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${SDL_MIXER}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_SOURCE_DIR}/deps/${FREETYPE}.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
endif()

# SDL supports cmake, but we have to do this because the sublibaries don't.
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${SDL})
ExternalProject_Add(
  sdl_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND ${MAKE}
)
add_library(sdl UNKNOWN IMPORTED)
set_property(TARGET sdl PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libSDL2.a)
add_dependencies(sdl sdl_autofail)

# Freetype
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${FREETYPE})
ExternalProject_Add(
  freetype_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR} --without-zlib --without-png --without-bzip2
  BUILD_COMMAND ${MAKE}
)
add_library(freetype UNKNOWN IMPORTED)
set_property(TARGET freetype PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libfreetype.a)
add_dependencies(freetype freetype_autofail)

# SDL TTF is not awesome
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${SDL_TTF})
ExternalProject_Add(
  sdl_ttf_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}  --with-sdl-prefix=${CMAKE_CURRENT_BINARY_DIR} --with-freetype-prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND ${MAKE}
)
add_library(sdl_ttf UNKNOWN IMPORTED)
set_property(TARGET sdl_ttf PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libSDL2_ttf.a)
add_dependencies(sdl_ttf sdl_ttf_autofail)
add_dependencies(sdl_ttf_autofail sdl freetype)

# SDL Image is not awesome
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${SDL_IMG})
ExternalProject_Add(
  sdl_img_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}  --with-sdl-prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND ${MAKE}
)
add_library(sdl_img UNKNOWN IMPORTED)
set_property(TARGET sdl_img PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libSDL2_image.a)
add_dependencies(sdl_img sdl_img_autofail)
add_dependencies(sdl_img_autofail sdl)

# SDL gfx is not awesome
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${SDL_GFX})
ExternalProject_Add(
  sdl_gfx_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}  --with-sdl-prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND ${MAKE}
)
add_library(sdl_gfx UNKNOWN IMPORTED)
set_property(TARGET sdl_gfx PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libSDL2_gfx.a)
add_dependencies(sdl_gfx sdl_gfx_autofail)
add_dependencies(sdl_gfx_autofail sdl)

# SDL mixer is not awesome
set(SRC ${CMAKE_CURRENT_BINARY_DIR}/src/${SDL_MIXER})
ExternalProject_Add(
  sdl_mixer_autofail
  SOURCE_DIR ${SRC}
  INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ${SRC}/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}  --with-sdl-prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_COMMAND ${MAKE}
)
add_library(sdl_mixer UNKNOWN IMPORTED)
set_property(TARGET sdl_mixer PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libSDL2_mixer.a)
add_dependencies(sdl_mixer sdl_mixer_autofail)
add_dependencies(sdl_mixer_autofail sdl)
if(APPLE)
  FIND_LIBRARY(AT_LIBRARY AudioToolbox)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${AT_LIBRARY})
endif()

# Leap library
add_library(leap UNKNOWN IMPORTED)
if(APPLE)
  set_property(TARGET leap PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/libleap.dylib)
else()
  if(UNIX) 
    set_property(TARGET leap PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/x64/libLeap.so)
  endif()
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Opengl
include(FindOpenGL)

# Collect source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c*)

# Includes
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${EXTRA_INCLUDES})

# Build lib
add_executable(${BIN} ${SOURCES})
target_link_libraries(${BIN} ${LIB})
target_link_libraries(${BIN} sdl sdl_ttf sdl_img sdl_mixer sdl_gfx freetype leap)
target_link_libraries(${BIN} ${OPENGL_LIBRARIES} ${OPENGL_glu_LIBRARY} ${EXTRA_LIBS})
if(APPLE)
  target_link_libraries(${BIN} -L${CMAKE_CURRENT_BINARY_DIR}/lib -lSDL2 -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit)
endif()
