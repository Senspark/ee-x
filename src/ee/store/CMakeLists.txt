cmake_minimum_required(VERSION 3.1)

project(ee_x_store)

include_directories(
  internal
)

set(LIB_SRC
  KeyValueStorage.cpp
  LifetimeVG.cpp
  MarketItem.cpp
  NativeKeyValueStorage.cpp
  PurchasableVirtualItem.cpp
  PurchaseType.cpp
  PurchaseWithMarket.cpp
  PurchaseWithVirtualItem.cpp
  SingleUsePackVG.cpp
  SingleUseVG.cpp
  StoreInfo.cpp
  StoreUtils.cpp
  VirtualCurrency.cpp
  VirtualCurrencyPack.cpp
  VirtualCurrencyStorage.cpp
  VirtualGood.cpp
  VirtualGoodsStorage.cpp
  VirtualItem.cpp
  VirtualItemStorage.cpp
)

set(LIB_HEADERS
  KeyValueStorage.hpp
  LifetimeVG.hpp
  MarketItem.hpp
  NativeKeyValueStorage.hpp
  PurchasableVirtualItem.hpp
  PurchaseType.hpp
  PurchaseWithMarket.hpp
  PurchaseWithVirtualItem.hpp
  SingleUsePackVG.hpp
  SingleUseVG.hpp
  StoreInfo.hpp
  StoreUtils.hpp
  VirtualCurrency.hpp
  VirtualCurrencyPack.hpp
  VirtualCurrencyStorage.hpp
  VirtualGood.hpp
  VirtualGoodsStorage.hpp
  VirtualItem.hpp
  VirtualItemStorage.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})