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

#include "CCSoomlaStoreConfigBuilder.h"

using namespace soomla;

CCSoomlaStoreConfigBuilder::CCSoomlaStoreConfigBuilder()
    : CCSoomlaConfigBuilder() {}

CCSoomlaStoreConfigBuilder* CCSoomlaStoreConfigBuilder::create() {
    return new CCSoomlaStoreConfigBuilder();
}

CCSoomlaStoreConfigBuilder* CCSoomlaStoreConfigBuilder::setIosConfiguration(
    CCSoomlaStoreIosConfigBuilder* iosConfig) {
    return this->appendConfigParameter(iosConfig->build()) ? this : NULL;
}

CCSoomlaStoreConfigBuilder* CCSoomlaStoreConfigBuilder::setGpConfiguration(
    CCSoomlaStoreGpConfigBuilder* gpConfig) {
    return this->appendConfigParameter(gpConfig->build()) ? this : NULL;
}
