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
set(BIN "sdl-test")

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

# SDL import
include(${CMAKE_CURRENT_SOURCE_DIR}/sdl.txt)

# Opengl
include(FindOpenGL)

# Collect source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Includes
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

# Link helper
function (link BIN) 
  target_link_libraries(${BIN} ${LIB})
  target_link_libraries(${BIN} sdl sdl_ttf sdl_img sdl_mixer sdl_gpu freetype)
  target_link_libraries(${BIN} ${OPENGL_LIBRARIES} ${EXTRA_LIBS})
  if(APPLE)
    target_link_libraries(${BIN} -L${CMAKE_CURRENT_BINARY_DIR}/lib -lSDL2 -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit)
  else()
    if(UNIX)
      target_link_libraries(${BIN} -L${CMAKE_CURRENT_BINARY_DIR}/lib -lSDL2 -lpthread -lm -ldl -lpthread -lrt)
    endif()
  endif()
endfunction()

# Example of plain SDL
add_executable(sdl-test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.c)
link(sdl-test)

# Example of sdl-gpu
add_executable(sdl-test-gpu ${CMAKE_CURRENT_SOURCE_DIR}/src/test_gpu.c)
link(sdl-test-gpu)
