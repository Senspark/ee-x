//
//  EEEventDispatcher.cpp
//  ee-library
//
//  Created by Zinge on 7/7/16.
//
//

#include "ee/cocos/EEEventDispatcher.hpp"

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListener.h>

namespace ee {
void EventDispatcher::addListener1(cocos2d::EventListener* listener) const {
    cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithFixedPriority(listener, 1);
}

void EventDispatcher::dispatch1(cocos2d::Event* event) const {
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(
        event);
}
} // namespace ee

#endif // EE_X_COCOS_CPP