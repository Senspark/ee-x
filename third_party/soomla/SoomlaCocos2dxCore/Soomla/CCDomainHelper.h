
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

#ifndef __CCDomainHelper_H_
#define __CCDomainHelper_H_

#include "CCDomain.h"
#include "cocos2d.h"

namespace soomla {
class CCDomainHelper {
public:
    static CCDomainHelper* getInstance();

    cocos2d::Vector<CCDomain*>
    getDomainsFromValueVector(const cocos2d::ValueVector& dictArray,
                            const std::string& type = "");
    cocos2d::ValueVector
    getValueVectorFromDomains(const cocos2d::Vector<CCDomain*>& domainArray);
};
} // namespace soomla

#endif //__CCDomainHelper_H_
