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

#ifndef __CCCoreEventDispatcher_H_
#define __CCCoreEventDispatcher_H_

#include "CCReward.h"
#include "cocos2d.h"

namespace soomla {

/** \class CCCoreEventDispatcher
    \brief Calls event handler functions when events are fired, also contains
   settings for StoreController.

    This class calls event handler functions when events are fired, to tie
    it to your event handler call addEventHandler(). You also set parameters
    for StoreController through this class.
 */
class CCCoreEventDispatcher : public cocos2d::Ref {
public:
    /**
       This class is singleton, access it with this function.
     */
    static CCCoreEventDispatcher* getInstance();

    bool init();

    /**
     Fires a custom event when a reward is given to the user.

     Event Name - CCCoreEvents::EVENT_REWARD_GIVEN
     Event Data (Dictionary):
     CCCoreEvents::DICT_ELEMENT_REWARD - CCReward - The reward given to the user
     */
    virtual void onRewardGivenEvent(CCReward* reward);
    /**
     Fires a custom event when a reward is taken to the user.

     Event Name - CCCoreEvents::EVENT_REWARD_TAKEN
     Event Data (Dictionary):
     CCCoreEvents::DICT_ELEMENT_REWARD - CCReward - The reward taken to the user
     */
    virtual void onRewardTakenEvent(CCReward* reward);
    /**
     Fires a custom event when a custom event is fired in Soomla.

     Event Name - CCCoreEvents::EVENT_CUSTOM
     Event Data (Dictionary):
     CCCoreEvents::DICT_ELEMENT_NAME - __String - The custom event name
     CCCoreEvents::DICT_ELEMENT_EXTRA - __Dictionary - The custom event
     contents in dictionary format
     */

    virtual void onCustomEvent(const std::string& name,
                               const cocos2d::ValueMap& extra);
};
}; // namespace soomla

#endif //__CCCoreEventDispatcher_H_
