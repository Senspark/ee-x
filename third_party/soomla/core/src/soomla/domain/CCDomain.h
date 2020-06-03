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

#ifndef __CCDomain_H_
#define __CCDomain_H_

#ifdef __cplusplus

#include <cocos2d.h>

namespace soomla {
class CCDomain : public cocos2d::Ref {
public:
    virtual bool initWithValueMap(const cocos2d::ValueMap& dict) = 0;
    virtual cocos2d::ValueMap toValueMap() = 0;
    virtual ~CCDomain() = default;

protected:
    cocos2d::Vector<CCDomain*>
    getDomainsFromDictArray(const cocos2d::ValueVector& dictArray);
    cocos2d::ValueVector
    getDictArrayFromDomains(const cocos2d::Vector<CCDomain*>& domainArray);
    cocos2d::ValueMap& putTypeData(cocos2d::ValueMap& dict,
                                  const std::string& key);
};
} // namespace soomla

#endif // __cplusplus

#endif //__CCDomain_H_
