cmake_minimum_required(VERSION 3.1)

project(soomla_cocos2dx_store)

include_directories(
    Soomla
    Soomla/config
    Soomla/data
    Soomla/domain
    Soomla/domain/virtualCurrencies
    Soomla/domain/virtualGoods
    Soomla/PurchaseTypes
    Soomla/NativeImpl
    Soomla/rewards
)

set(EE_X_SOOMLA_STORE_SRC
    Soomla/NativeImpl/CCNativeStoreInfo.cpp
    Soomla/NativeImpl/CCNativeVirtualCurrencyStorage.cpp
    Soomla/NativeImpl/CCNativeStoreBridge.cpp
    Soomla/NativeImpl/CCNativeVirtualGoodsStorage.cpp
    Soomla/NativeImpl/CCNativeSoomlaStore.cpp
    Soomla/PurchaseTypes/CCPurchaseWithMarket.cpp
    Soomla/PurchaseTypes/CCPurchaseType.cpp
    Soomla/PurchaseTypes/CCPurchaseWithVirtualItem.cpp
    Soomla/PurchaseTypes/CCPurchaseWithMarketBuilder.cpp
    Soomla/CCStoreBridge.cpp
    Soomla/CCSoomlaStore.cpp
    Soomla/CCStoreInventory.cpp
    Soomla/config/CCSoomlaStoreGpConfigBuilder.cpp
    Soomla/config/CCSoomlaStoreIosConfigBuilder.cpp
    Soomla/config/CCSoomlaStoreConfigBuilder.cpp
    Soomla/CCStoreConsts.cpp
    Soomla/rewards/CCVirtualItemReward.cpp
    Soomla/CCStoreEventDispatcher.cpp
    Soomla/StoreEventListener.cpp
    Soomla/data/CCVirtualItemStorage.cpp
    Soomla/data/CCVirtualGoodsStorage.cpp
    Soomla/data/CCStoreInfo.cpp
    Soomla/data/CCVirtualCurrencyStorage.cpp
    Soomla/domain/CCMarketItem.cpp
    Soomla/domain/virtualCurrencies/CCVirtualCurrency.cpp
    Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.cpp
    Soomla/domain/virtualGoods/CCVirtualGood.cpp
    Soomla/domain/virtualGoods/CCSingleUseVG.cpp
    Soomla/domain/virtualGoods/CCUpgradeVG.cpp
    Soomla/domain/virtualGoods/CCSingleUsePackVG.cpp
    Soomla/domain/virtualGoods/CCEquippableVG.cpp
    Soomla/domain/virtualGoods/CCLifetimeVG.cpp
    Soomla/domain/CCPurchasableVirtualItem.cpp
    Soomla/domain/CCVirtualCategory.cpp
    Soomla/domain/CCVirtualItem.cpp
)

set(EE_X_SOOMLA_STORE_HEADERS
    Soomla/NativeImpl/CCNativeVirtualGoodsStorage.h
    Soomla/NativeImpl/CCNativeStoreInfo.h
    Soomla/NativeImpl/CCNativeStoreBridge.h
    Soomla/NativeImpl/CCNativeSoomlaStore.h
    Soomla/NativeImpl/CCNativeVirtualCurrencyStorage.h
    Soomla/PurchaseTypes/CCPurchaseWithMarketBuilder.h
    Soomla/PurchaseTypes/CCPurchaseWithMarket.h
    Soomla/PurchaseTypes/CCPurchaseWithVirtualItem.h
    Soomla/PurchaseTypes/CCPurchaseType.h
    Soomla/Cocos2dxStore.h
    Soomla/CCStoreBridge.h
    Soomla/CCSoomlaStore.h
    Soomla/config/CCSoomlaStoreIosConfigBuilder.h
    Soomla/config/CCSoomlaStoreGpConfigBuilder.h
    Soomla/config/CCSoomlaStoreConfigBuilder.h
    Soomla/rewards/CCVirtualItemReward.h
    Soomla/CCStoreAssets.h
    Soomla/CCStoreInventory.h
    Soomla/CCStoreEventDispatcher.h
    Soomla/StoreEventListener.h
    Soomla/CCStoreConsts.h
    Soomla/data/CCVirtualCurrencyStorage.h
    Soomla/data/CCVirtualItemStorage.h
    Soomla/data/CCStoreInfo.h
    Soomla/data/CCVirtualGoodsStorage.h
    Soomla/domain/CCVirtualItem.h
    Soomla/domain/CCPurchasableVirtualItem.h
    Soomla/domain/CCMarketItem.h
    Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h
    Soomla/domain/virtualCurrencies/CCVirtualCurrency.h
    Soomla/domain/virtualGoods/CCVirtualGood.h
    Soomla/domain/virtualGoods/CCEquippableVG.h
    Soomla/domain/virtualGoods/CCLifetimeVG.h
    Soomla/domain/virtualGoods/CCSingleUsePackVG.h
    Soomla/domain/virtualGoods/CCUpgradeVG.h
    Soomla/domain/virtualGoods/CCSingleUseVG.h
    Soomla/domain/CCVirtualCategory.h
)

add_library(${PROJECT_NAME} STATIC ${EE_X_SOOMLA_STORE_SRC} ${EE_X_SOOMLA_STORE_HEADERS})

add_subdirectory(../SoomlaCocos2dxCore "${CMAKE_BINARY_DIR}/lib/soomla_cocos2dx_core")

target_link_libraries(${PROJECT_NAME} soomla_cocos2dx_core)

target_include_directories(${PROJECT_NAME} PUBLIC
    Soomla
    Soomla/config
    Soomla/data
    Soomla/domain
    Soomla/domain/virtualCurrencies
    Soomla/domain/virtualGoods
    Soomla/PurchaseTypes
    Soomla/NativeImpl
    Soomla/rewards
)
