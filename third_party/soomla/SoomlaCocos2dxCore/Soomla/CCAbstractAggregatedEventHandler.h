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

#ifndef __CCAbstractAggregatedEventHandler_H_
#define __CCAbstractAggregatedEventHandler_H_

#include <cocos2d.h>

namespace soomla {

#define FOR_EACH_EVENT_HANDLER(__TYPE__)                                       \
    __SetIterator i;                                                           \
    for (i = mEventHandlers->begin(); i != mEventHandlers->end(); i++) {       \
        __TYPE__* eventHandler = dynamic_cast<__TYPE__*>(*i);

template <class T>
class CCAbstractAggregatedEventHandler {
protected:
    cocos2d::__Set* mEventHandlers;
    cocos2d::__Set* tempAddEventHandlers;
    cocos2d::__Set* tempRemoveEventHandlers;
    int lockCount;

public:
    CCAbstractAggregatedEventHandler()
        : mEventHandlers(nullptr)
        , tempAddEventHandlers(nullptr)
        , tempRemoveEventHandlers(nullptr)
        , lockCount(0) {}
    virtual bool init() {
        mEventHandlers = cocos2d::__Set::create();
        mEventHandlers->retain();

        tempAddEventHandlers = cocos2d::__Set::create();
        tempAddEventHandlers->retain();

        tempRemoveEventHandlers = cocos2d::__Set::create();
        tempRemoveEventHandlers->retain();

        return true;
    }

    virtual ~CCAbstractAggregatedEventHandler() {
        CC_SAFE_RELEASE(mEventHandlers);
    }

    /**
    Add an event handler. This retains the event handler.
    \param eventHandler A pointer to the event handler you'd like to add.
    */
    void addEventHandler(T* eventHandler) {
        if (lockCount <= 0) {
            mEventHandlers->addObject(eventHandler);
        } else {
            tempAddEventHandlers->addObject(eventHandler);
        }
    }

    /**
    Remove an event handler. This releases the event handler.
    \param eventHandler A pointer to the event handler you'd like to remove.
    */
    void removeEventHandler(T* eventHandler) {
        if (lockCount <= 0) {
            mEventHandlers->removeObject(eventHandler);
        } else {
            tempRemoveEventHandlers->addObject(eventHandler);
        }
    }

    /**
     Removes all event handlers from the event handler
     */
    void purge() {
        mEventHandlers->removeAllObjects();

        lockCount = 0;

        tempAddEventHandlers->removeAllObjects();
        tempRemoveEventHandlers->removeAllObjects();
    }

protected:
    void lockEventHandlers() { lockCount++; }

    void unlockEventHandlers() {
        lockCount--;

        if (lockCount <= 0) {
            for (cocos2d::__SetIterator i = tempAddEventHandlers->begin();
                 i != tempAddEventHandlers->end(); i++) {
                if (!(*i)) {
                    break;
                }

                T* addHandler = dynamic_cast<T*>(*i);
                addEventHandler(addHandler);
            }

            for (cocos2d::__SetIterator i = tempRemoveEventHandlers->begin();
                 i != tempRemoveEventHandlers->end(); i++) {
                if (!(*i)) {
                    break;
                }

                T* removeHandler = dynamic_cast<T*>(*i);
                removeEventHandler(removeHandler);
            }

            tempAddEventHandlers->removeAllObjects();
            tempRemoveEventHandlers->removeAllObjects();

            lockCount = 0;
        }
    }
};
} // namespace soomla

#endif //__CCAbstractAggregatedEventHandler_H_
