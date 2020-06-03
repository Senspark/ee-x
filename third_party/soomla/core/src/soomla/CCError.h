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

#ifndef __CCError_H_
#define __CCError_H_

#ifdef __cplusplus

#include <cocos2d.h>

namespace soomla {

class CCError : public cocos2d::Ref {
public:
    static CCError* createWithValue(const cocos2d::Value& value);
    static void tryFillError(CCError** error, const cocos2d::Value& value,
                             const char* tag = nullptr);
    const char* getInfo() { return mInfo.c_str(); };

private:
    /**
       Extra information about what went wrong
     */
    std::string mInfo;

    CCError()
        : mInfo("") {}

    bool init(std::string& errorInfo);
};
}; // namespace soomla

#endif // __cplusplus

#endif //__CCError_H_
