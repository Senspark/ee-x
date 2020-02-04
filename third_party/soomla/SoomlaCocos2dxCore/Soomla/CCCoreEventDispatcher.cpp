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

#include "Soomla/CCCoreEventDispatcher.h"

#include "Soomla/CCCoreConsts.h"
#include "Soomla/CCDomainFactory.h"
#include "Soomla/CCSoomlaEventDispatcher.h"

#if __has_include(<base/CCDirector.h>)
#define COCOSCREATOR_VERSION 1
#else
#define COCOSCREATOR_VERSION 2
#endif

#if COCOSCREATOR_VERSION == 2
#include "cocos/scripting/js-bindings/event/EventDispatcher.h"
#endif

namespace soomla {

USING_NS_CC;

static CCCoreEventDispatcher* s_SharedInstance = nullptr;

CCCoreEventDispatcher* CCCoreEventDispatcher::getInstance() {
    if (!s_SharedInstance) {
        s_SharedInstance = new CCCoreEventDispatcher();
        s_SharedInstance->init();
    }

    return s_SharedInstance;
}

bool CCCoreEventDispatcher::init() {
    CCSoomlaEventDispatcher* eventDispatcher =
        CCSoomlaEventDispatcher::getInstance();

    eventDispatcher->registerEventHandler(
        CCCoreConsts::EVENT_REWARD_GIVEN, [this](const ValueMap& parameters) {
            auto&& rewardId =
                parameters.at(CCCoreConsts::DICT_ELEMENT_REWARD).asString();
            CCReward* reward = CCReward::getReward(rewardId);
            this->onRewardGivenEvent(reward);
        });
    eventDispatcher->registerEventHandler(
        CCCoreConsts::EVENT_REWARD_TAKEN, [this](const ValueMap& parameters) {
            auto&& rewardId =
                parameters.at(CCCoreConsts::DICT_ELEMENT_REWARD).asString();
            CCReward* reward = CCReward::getReward(rewardId);
            this->onRewardTakenEvent(reward);
        });
    eventDispatcher->registerEventHandler(
        CCCoreConsts::EVENT_CUSTOM, [this](const ValueMap& parameters) {
            auto&& name =
                parameters.at(CCCoreConsts::DICT_ELEMENT_NAME).asString();
            auto& extra =
                parameters.at(CCCoreConsts::DICT_ELEMENT_EXTRA).asValueMap();
            this->onCustomEvent(name, extra);
        });

    return true;
}

void CCCoreEventDispatcher::onRewardGivenEvent(CCReward* reward) {
    ValueMap eventMap;
    eventMap[CCCoreConsts::DICT_ELEMENT_REWARD] = reward->toValueMap();
    
#if COCOSCREATOR_VERSION == 2
    cocos2d::CustomEvent event;
    event.name = CCCoreConsts::EVENT_REWARD_GIVEN;
    event.args[0].ptrVal = &eventMap;
    EventDispatcher::dispatchCustomEvent(event);
#elif COCOSCREATOR_VERSION == 1
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
    CCCoreConsts::EVENT_REWARD_GIVEN , &eventMap);
#endif
}

void CCCoreEventDispatcher::onRewardTakenEvent(CCReward* reward) {
    ValueMap eventMap;
    eventMap[CCCoreConsts::DICT_ELEMENT_REWARD] = reward->toValueMap();

#if COCOSCREATOR_VERSION == 2
    cocos2d::CustomEvent event;
    event.name = CCCoreConsts::EVENT_REWARD_TAKEN;
    event.args[0].ptrVal = &eventMap;
    EventDispatcher::dispatchCustomEvent(event);
#elif COCOSCREATOR_VERSION == 1
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
    CCCoreConsts::EVENT_REWARD_TAKEN, &eventMap);
#endif
    
}

void CCCoreEventDispatcher::onCustomEvent(const std::string& name,
                                          const cocos2d::ValueMap& extra) {
    ValueMap eventDict;
    eventDict[CCCoreConsts::DICT_ELEMENT_NAME] = name;
    eventDict[CCCoreConsts::DICT_ELEMENT_EXTRA] = extra;
    
#if COCOSCREATOR_VERSION == 2
    cocos2d::CustomEvent event;
    event.name = CCCoreConsts::EVENT_CUSTOM;
    event.args[0].ptrVal = &eventDict;
    EventDispatcher::dispatchCustomEvent(event);
#elif COCOSCREATOR_VERSION == 1
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
        CCCoreConsts::EVENT_CUSTOM, &eventDict);
#endif
}
} // namespace soomla
