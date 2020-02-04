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

#include "Soomla/CCDomainHelper.h"

#include "Soomla/CCCoreConsts.h"
#include "Soomla/CCDomainFactory.h"
#include "Soomla/CCJsonHelper.h"

using namespace soomla;
using namespace cocos2d;

static CCDomainHelper* s_SharedDomainFactory = nullptr;

CCDomainHelper* CCDomainHelper::getInstance() {
    if (!s_SharedDomainFactory) {
        s_SharedDomainFactory = new CCDomainHelper();
    }

    return s_SharedDomainFactory;
}

cocos2d::Vector<CCDomain*> soomla::CCDomainHelper::getDomainsFromValueVector(
    const cocos2d::ValueVector& dictArray, const std::string& type) {

    if (dictArray == ValueVectorNull) {
        return Vector<CCDomain*>();
    }

    Vector<CCDomain*> domainArray;
    
    for (auto& obj : dictArray) {
        CCDomain* domain = nullptr;
        if (type.length() == 0) {
            domain =
                soomla::CCDomainFactory::getInstance()->createWithValueMap(
                    obj.asValueMap());
        } else {
            domain = soomla::CCDomainFactory::getInstance()
                         ->createWithValueMapAndType(obj.asValueMap(), type);
        }
        domainArray.pushBack(domain);
    }
    
    return domainArray;
}

cocos2d::ValueVector soomla::CCDomainHelper::getValueVectorFromDomains(
    const cocos2d::Vector<CCDomain*>& domainArray) {
    if (domainArray.size() == 0) {
        return ValueVectorNull;
    }
    
    ValueVector dictArray;

    for (auto& obj : domainArray) {
        dictArray.push_back(Value(obj->toValueMap()));
    }

    return dictArray;
}

