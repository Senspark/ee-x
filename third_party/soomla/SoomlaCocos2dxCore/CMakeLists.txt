cmake_minimum_required(VERSION 3.1)

project(soomla_cocos2dx_core)

include_directories(
    Soomla
    Soomla/data
    Soomla/domain
    Soomla/rewards
    Soomla/NativeImpl
)

set(LIB_SRC
    Soomla/NativeImpl/CCNativeCoreBridge.cpp
    Soomla/NativeImpl/CCNativeKeyValueStorage.cpp
    Soomla/NativeImpl/CCNativeRewardStorage.cpp
    Soomla/CCError.cpp
    Soomla/CCCoreEventDispatcher.cpp
    Soomla/CCDomainFactory.cpp
    Soomla/CCDomainHelper.cpp
    Soomla/CCJsonHelper.cpp
    Soomla/rewards/CCRandomReward.cpp
    Soomla/rewards/CCAggregateReward.cpp
    Soomla/rewards/CCReward.cpp
    Soomla/rewards/CCSequenceReward.cpp
    Soomla/rewards/CCBadgeReward.cpp
    Soomla/CCSoomla.cpp
    Soomla/CCCoreBridge.cpp
    Soomla/CCSoomlaEventDispatcher.cpp
    Soomla/CCSoomlaConfigBuilder.cpp
    Soomla/CCSoomlaUtils.cpp
    Soomla/data/CCRewardStorage.cpp
    Soomla/data/CCKeyValueStorage.cpp
    Soomla/data/CCBridgelessKeyValueStorage.cpp
    Soomla/domain/CCDomain.cpp
    Soomla/domain/CCSoomlaEntity.cpp
    Soomla/CCCoreConsts.cpp
    Soomla/CCSchedule.cpp
    Soomla/CCNdkBridge.cpp
)

set(LIB_HEADERS
    Soomla/NativeImpl/CCNativeCoreBridge.h
    Soomla/NativeImpl/CCNativeKeyValueStorage.h
    Soomla/NativeImpl/CCNativeRewardStorage.h
    Soomla/CCAbstractAggregatedEventHandler.h
    Soomla/CCNdkBridge.h
    Soomla/CCSoomlaMacros.h
    Soomla/Cocos2dxCore.h
    Soomla/CCDomainHelper.h
    Soomla/CCCoreBridge.h
    Soomla/rewards/CCReward.h
    Soomla/rewards/CCAggregateReward.h
    Soomla/rewards/CCBadgeReward.h
    Soomla/rewards/CCRandomReward.h
    Soomla/rewards/CCSequenceReward.h
    Soomla/CCSoomlaEventDispatcher.h
    Soomla/CCError.h
    Soomla/CCCoreEventDispatcher.h
    Soomla/CCJsonHelper.h
    Soomla/CCDomainFactory.h
    Soomla/CCSoomla.h
    Soomla/CCSoomlaUtils.h
    Soomla/CCSoomlaConfigBuilder.h
    Soomla/CCCoreConsts.h
    Soomla/data/CCRewardStorage.h
    Soomla/data/CCBridgelessKeyValueStorage.h
    Soomla/data/CCKeyValueStorage.h
    Soomla/domain/CCSoomlaEntity.h
    Soomla/domain/CCDomain.h
    Soomla/CCSchedule.h
)

add_library(${PROJECT_NAME} STATIC ${LIB_SRC} ${LIB_HEADERS})

add_subdirectory(../../jansson "${CMAKE_BINARY_DIR}/lib/jansson")

target_link_libraries(${PROJECT_NAME} jansson)

target_include_directories(${PROJECT_NAME} PUBLIC
    Soomla
    Soomla/domain
    Soomla/rewards
    Soomla/data
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)