cmake_minimum_required(VERSION 3.1)

project(ee_x)

add_subdirectory(src/cpp/ee)
add_subdirectory(src/cpp/ee/soomla)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/soomla/core)
add_subdirectory(third_party/soomla/store)

#ifdef __JETBRAINS_IDE__
target_compile_definitions(${PROJECT_NAME} PUBLIC __ANDROID__)
#endif