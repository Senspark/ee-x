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

#ifndef __CCEventDispatcher_H_
#define __CCEventDispatcher_H_

#ifdef __cplusplus

#include <cocos2d.h>

namespace soomla {

class CCSoomlaEventDispatcher : public cocos2d::Ref {
private:
    std::map<std::string, std::function<void(const cocos2d::ValueMap&)>>
        mEventHandlers;

public:
    static CCSoomlaEventDispatcher* getInstance();

    void ndkCallback(const cocos2d::ValueMap& parameters);
    void
    registerEventHandler(const std::string& key,
                         std::function<void(const cocos2d::ValueMap&)> handler);
    void unregisterEventHander(const std::string& key);
};
}; // namespace soomla

#endif // __cplusplus

#endif //__CCEventDispatcher_H_
