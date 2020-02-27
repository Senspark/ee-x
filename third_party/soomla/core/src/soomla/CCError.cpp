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

#include "soomla/CCError.h"

#include "soomla/CCSoomlaUtils.h"

USING_NS_CC;

namespace soomla {
#define CANNOT_EXTRACT "Cannot extract error data"
#define JSON_ERROR_INFO "errorInfo"

#define TAG "CCError"

CCError* CCError::createWithValue(const cocos2d::Value& value) {
    if (value == cocos2d::Value::Null) {
        return nullptr;
    }

    std::string errorInfoStr = "";
    if (value.getType() == cocos2d::Value::Type::STRING) {
        errorInfoStr = value.asString();
    } else if (value.getType() == cocos2d::Value::Type::MAP) {
        if (!value.asValueMap().count(JSON_ERROR_INFO)) {
            return nullptr;
        }
        errorInfoStr = value.asValueMap().at(JSON_ERROR_INFO).asString();
    }

    if (errorInfoStr.length() == 0) {
        errorInfoStr = CANNOT_EXTRACT;
    }

    CCError* ret = new CCError();
    if (ret->init(errorInfoStr)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CCError::tryFillError(soomla::CCError **error, const cocos2d::Value &value, const char* tag) {
    if (error != nullptr) {
        CCError* resultError = CCError::createWithValue(value);
        if (resultError != nullptr) {
            *error = resultError;
        }
    } else {
        auto&& errorStr = value.asString();
        if (errorStr.length() > 0) {
            if (tag == nullptr) {
                tag = TAG;
            }
            CCSoomlaUtils::logError(tag, errorStr);
        }
    }
}

bool CCError::init(std::string& errorInfo) {
    mInfo = errorInfo;

    return true;
}
} // namespace soomla
