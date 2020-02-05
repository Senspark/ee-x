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

#include "Soomla/CCSoomlaEventDispatcher.h"

#include "Soomla/CCJsonHelper.h"

#ifdef COCOS2D_JAVASCRIPT
#include "JSBinding.h"
#endif

namespace soomla {

USING_NS_CC;

static CCSoomlaEventDispatcher* s_SharedInstance = nullptr;

CCSoomlaEventDispatcher* CCSoomlaEventDispatcher::getInstance() {
    if (!s_SharedInstance) {
        s_SharedInstance = new CCSoomlaEventDispatcher();
    }

    return s_SharedInstance;
}

void CCSoomlaEventDispatcher::registerEventHandler(
    const std::string& key,
    std::function<void(const cocos2d::ValueMap&)> handler) {
    mEventHandlers[key] = handler;
}

void CCSoomlaEventDispatcher::unregisterEventHander(const std::string& key) {
    mEventHandlers.erase(key);
}

void CCSoomlaEventDispatcher::ndkCallback(const cocos2d::ValueMap& parameters) {
#ifdef COCOS2D_JAVASCRIPT
    Soomla::JSBinding::callCallback(parameters);
#else
    if (not parameters.count("method")) {
        // Suppress any events without callbacks (push event probably)
        return;
    }

    auto&& eventName = parameters.at("method").asString();
    std::function<void(const ValueMap&)> handler = mEventHandlers[eventName];

    if (handler) {
        handler(parameters);
    } else {
        log("Unregistered event happened: %s", eventName.c_str());
    }
#endif
}
} // namespace soomla
