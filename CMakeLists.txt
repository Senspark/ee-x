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
  ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include/ee/nlohmann
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/admob
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/analytics
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/auth
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/database
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/dynamic_links
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/internal
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/storage
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/google_play_services
  ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include
)

set(GAME_SRC
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/JniUtils.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniMethodInfo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniString.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/Logger_Android.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/MessageBridge_Android.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/firebase/FirebaseApp.cpp
)

set(GAME_HEADERS_INCLUDE
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/JniUtils.hpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JnimethodInfo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniString.hpp
   ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include/ee/nlohmann/json.hpp
)

include(${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/core/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/crashlytics/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/google/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/notification/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/store/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/CMakeLists.txt)

add_library(ee-x STATIC ${GAME_SRC} ${GAME_HEADERS_INCLUDE})