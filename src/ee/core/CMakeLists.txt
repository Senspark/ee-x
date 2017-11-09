#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/
cmake_minimum_required(VERSION 3.1)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/src/ee
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/core
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal
)

set(GAME_SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Logger.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/LogLevel.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Metrics.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Utils.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/MessageBridge.cpp
)

set(GAME_HEADERS_INCLUDE
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Core.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/CoreFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Logger.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/LogLevel.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Metrics.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Utils.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/MessageBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/ScopeGuard.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/SpinLock.hpp
)

add_library(ee-x STATIC ${GAME_SRC} ${GAME_HEADERS_INCLUDE})