cmake_minimum_required(VERSION 3.1)

project(ee_x)

add_subdirectory(src/cpp/ee)
add_subdirectory(src/cpp/ee/soomla)
add_subdirectory(third_party/firebase_cpp_sdk)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/soomla/core)
add_subdirectory(third_party/soomla/store)