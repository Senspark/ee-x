/*
 Copyright (C) 2012-2015 Soomla Inc.

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

#include "CCSoomlaConfigBuilder.h"
#include <string>
#include "CCJsonHelper.h"

using namespace soomla;

CCSoomlaConfigBuilder* CCSoomlaConfigBuilder::create() {
    return new CCSoomlaConfigBuilder();
}

bool CCSoomlaConfigBuilder::appendConfigParameter(const std::string& key,
                                                  const cocos2d::Value& value) {
    _rawConfig[key] = value;
    return true;
}

bool CCSoomlaConfigBuilder::appendConfigParameter(const cocos2d::ValueMap &map) {
    for (auto element : map) {
        _rawConfig[element.first] = element.second;
    }
    return true;
}

const cocos2d::ValueMap& CCSoomlaConfigBuilder::build() {
    return this->_rawConfig;
}
