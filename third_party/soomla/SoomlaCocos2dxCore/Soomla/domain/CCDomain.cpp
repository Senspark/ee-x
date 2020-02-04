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

#include "Soomla/domain/CCDomain.h"

#include "Soomla/CCCoreConsts.h"
#include "Soomla/CCDomainHelper.h"
#include "Soomla/CCJsonHelper.h"

cocos2d::Vector<soomla::CCDomain*> soomla::CCDomain::getDomainsFromDictArray(
    const cocos2d::ValueVector& dictArray) {
    return CCDomainHelper::getInstance()->getDomainsFromValueVector(dictArray);
}

cocos2d::ValueVector soomla::CCDomain::getDictArrayFromDomains(
    const cocos2d::Vector<CCDomain*>& domainArray) {
    return CCDomainHelper::getInstance()->getValueVectorFromDomains(domainArray);
}

cocos2d::ValueMap& soomla::CCDomain::putTypeData(cocos2d::ValueMap& dict,
                                                const std::string& type) {
    dict[CCCoreConsts::JSON_JSON_TYPE] = type;
    return dict;
}
