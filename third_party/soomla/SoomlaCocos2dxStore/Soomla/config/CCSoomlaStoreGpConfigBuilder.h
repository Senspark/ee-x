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

#ifndef COCOS2DXSTORE_CCSOOMLASTOREGPCONFIGBUILDER_H
#define COCOS2DXSTORE_CCSOOMLASTOREGPCONFIGBUILDER_H

#include <Soomla/CCSoomlaConfigBuilder.h>

namespace soomla {
class CCSoomlaStoreGpConfigBuilder : public CCSoomlaConfigBuilder {
public:
    CCSoomlaStoreGpConfigBuilder();
    static CCSoomlaStoreGpConfigBuilder* create();
    CCSoomlaStoreGpConfigBuilder*
    setAndroidPublicKey(const std::string& androidPublicKey);
    CCSoomlaStoreGpConfigBuilder* setTestPurchases(bool isTestPurchases);
    CCSoomlaStoreGpConfigBuilder* activateFraudProtection(
        const std::string& clientId, const std::string& clientSecret,
        const std::string& refreshToken, bool verifyOnServerFailure);
};
} // namespace soomla

#endif // COCOS2DXSTORE_CCSOOMLASTOREGPCONFIGBUILDER_H
