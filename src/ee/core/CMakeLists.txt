cmake_minimum_required(VERSION 3.1)

# project(ee_x_core)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
  src/ee/core
  src/ee/core/internal
  third_party/nlohmann/include
)

set(EE_X_CORE_SRC
  src/ee/core/Logger.cpp
  src/ee/core/LogLevel.cpp
  src/ee/core/Metrics.cpp
  src/ee/core/Utils.cpp
  src/ee/core/MessageBridge.cpp
)

set(EE_X_CORE_HEADERS
  src/ee/core/Logger.hpp
  src/ee/core/LogLevel.hpp
  src/ee/core/Metrics.hpp
  src/ee/core/Utils.hpp
  src/ee/core/MessageBridge.hpp
  src/ee/core/internal/ScopeGuard.hpp
  src/ee/core/internal/SpinLock.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
