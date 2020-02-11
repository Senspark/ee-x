/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "soomla/data/CCKeyValueStorage.h"

#include "soomla/data/CCBridgelessKeyValueStorage.h"
#include "soomla/NativeImpl/CCNativeKeyValueStorage.h"

namespace soomla {
static CCKeyValueStorage* s_SharedKeyValueStorage = nullptr;

CCKeyValueStorage* CCKeyValueStorage::getInstance() {
    if (!s_SharedKeyValueStorage) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        s_SharedKeyValueStorage = new CCNativeKeyValueStorage();
#else
        s_SharedKeyValueStorage = new CCBridgelessKeyValueStorage();
#endif
        s_SharedKeyValueStorage->retain();
    }

    return s_SharedKeyValueStorage;
}
} // namespace soomla
