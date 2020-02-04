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

#ifndef __CCDomainFactory_H_
#define __CCDomainFactory_H_

#include <cocos2d.h>

#include "Soomla/domain/CCDomain.h"

namespace soomla {
class CCDomainFactory {
public:
    static CCDomainFactory* getInstance();
    void registerCreator(
        const std::string& key,
        std::function<soomla::CCDomain*(const cocos2d::ValueMap&)> creator);
    soomla::CCDomain* createWithValueMap(const cocos2d::ValueMap& dict);
    soomla::CCDomain* createWithValueMapAndType(const cocos2d::ValueMap& dict,
                                                  const std::string& type);

private:
    std::map<std::string,
             std::function<soomla::CCDomain*(const cocos2d::ValueMap&)>>
        mCreators;
};
} // namespace soomla

#endif //__CCDomainFactory_H_
