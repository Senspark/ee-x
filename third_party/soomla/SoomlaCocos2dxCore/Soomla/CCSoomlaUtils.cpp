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



#include "Soomla/CCSoomlaUtils.h"

#include <cocos2d.h>

namespace soomla {
    void CCSoomlaUtils::logDebug(const std::string& tag, const std::string& message) {
        if (SOOMLA_DEBUG) {
            cocos2d::log("%s %s", tag.c_str(), message.c_str());
        }
    }
    void CCSoomlaUtils::logError(const std::string& tag, const std::string& message) {
        CCLOGERROR("%s %s", tag.c_str(), message.c_str());
    }

    void CCSoomlaUtils::logException(const std::string& tag, CCError *error) {
        CC_ASSERT(error);
        cocos2d::log("%s Soomla error: %s", tag.c_str(), error->getInfo());
    }

    bool CCSoomlaUtils::isEmpty(const std::string& str) {
        return str.length() == 0;
    }
};
