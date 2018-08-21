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

#include "CCCoreConsts.h"
#include "CCDomainFactory.h"
#include "CCJsonHelper.h"

USING_NS_CC;

using namespace soomla;

static CCDomainFactory* s_SharedDomainFactory = nullptr;

CCDomainFactory* CCDomainFactory::getInstance() {
    if (!s_SharedDomainFactory) {
        s_SharedDomainFactory = new CCDomainFactory();
    }

    return s_SharedDomainFactory;
}

void CCDomainFactory::registerCreator(
    const std::string& key,
    std::function<soomla::CCDomain*(const cocos2d::ValueMap&)> creator) {
    mCreators[key] = creator;
}

soomla::CCDomain*
CCDomainFactory::createWithValueMap(const cocos2d::ValueMap& dict) {
    auto& typeValue = dict.at(CCCoreConsts::JSON_JSON_TYPE);
    CC_ASSERT(typeValue != Value::Null);
    auto&& type = typeValue.asString();

    return this->createWithValueMapAndType(dict, type);
}

soomla::CCDomain*
CCDomainFactory::createWithValueMapAndType(const cocos2d::ValueMap& dict,
                                             const std::string& type) {
    auto creator = mCreators[type];
    CC_ASSERT(creator);
    if (creator == nullptr) {
        return nullptr;
    }

    return creator(dict);
}
