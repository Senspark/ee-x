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
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/store
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/internal
)

set(GAME_SRC
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/KeyValueStorage.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/LifetimeVG.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/MarketItem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/NativeKeyValueStorage.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchasableVirtualItem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseType.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseWithMarket.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseWithVirtualItem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/SingleUsePackVG.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/SingleUseVG.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/Store.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreInfo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreLogger.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreUtils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrency.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrencyPack.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrencyStorage.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualGood.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualGoodsStorage.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualItem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualItemStorage.cpp
)

set(GAME_HEADERS_INCLUDE
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/KeyValueStorage.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/LifetimeVG.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/MarketItem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/NativeKeyValueStorage.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchasableVirtualItem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseType.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseWithMarket.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/PurchaseWithVirtualItem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/SingleUsePackVG.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/SingleUseVG.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/Store.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreInfo.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreLogger.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/StoreUtils.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrency.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrencyPack.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualCurrencyStorage.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualGood.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualGoodsStorage.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualItem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/store/VirtualItemStorage.hpp
)