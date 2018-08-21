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

#include "CCJsonHelper.h"

USING_NS_CC;
using namespace std;

cocos2d::Value CCJsonHelper::getValueFromJson(json_t* obj) {
    if (obj == NULL) {
        return cocos2d::Value::Null;
    }

    if (json_is_object(obj)) {
        cocos2d::ValueMap dictionary;

        const char* key;
        json_t* value;

        void* iter = json_object_iter(obj);
        while (iter) {
            key = json_object_iter_key(iter);
            value = json_object_iter_value(iter);
            dictionary[key] = CCJsonHelper::getValueFromJson(value);
            iter = json_object_iter_next(obj, iter);
        }
        return cocos2d::Value(dictionary);
    } else if (json_is_array(obj)) {
        size_t sizeArray = json_array_size(obj);
        cocos2d::ValueVector array;
        for (unsigned int i = 0; i < sizeArray; i++) {
            array.push_back(
                CCJsonHelper::getValueFromJson(json_array_get(obj, i)));
        }
        return cocos2d::Value(array);
    } else if (json_is_boolean(obj)) {
        bool ccBool = json_is_true(obj);
        return cocos2d::Value(ccBool);
    } else if (json_is_integer(obj)) {
        json_int_t intVal = json_integer_value(obj);

        if (intVal >= std::numeric_limits<int>::min() &&
            intVal <= std::numeric_limits<int>::max()) {
            return cocos2d::Value(static_cast<int>(intVal));
        }

        return cocos2d::Value(static_cast<double>(intVal));
    } else if (json_is_real(obj)) {
        double realVal = json_real_value(obj);
        return cocos2d::Value(realVal);
    } else if (json_is_string(obj)) {
        std::stringstream str;
        str << json_string_value(obj);
        string ret = str.str();
        return cocos2d::Value(ret);
    } else if (json_is_null(obj)) {
        return cocos2d::Value::Null;
    } else {
        CC_ASSERT(false);
        return cocos2d::Value::Null;
    }
}

json_t* CCJsonHelper::getJsonFromValue(const cocos2d::Value& value) {
    if (value.getType() == cocos2d::Value::Type::MAP) {
        auto& mainDict = value.asValueMap();
        json_t* jsonDict = json_object();
        if (mainDict.size() == 0)
            return jsonDict;

        for (auto& ele : mainDict) {
            auto key = ele.first.c_str();
            json_object_set_new(jsonDict, key,
                                CCJsonHelper::getJsonFromValue(ele.second));
        }
        return jsonDict;
    } else if (value.getType() == cocos2d::Value::Type::VECTOR) {
        auto& mainArray = value.asValueVector();
        json_t* jsonArray = json_array();

        for (auto& ele : mainArray) {
            json_array_append_new(jsonArray,
                                  CCJsonHelper::getJsonFromValue(ele));
        }

        return jsonArray;
    } else if (value.getType() == cocos2d::Value::Type::STRING) {
        json_t* jsonString = json_string(value.asString().c_str());
        return jsonString;
    } else if (value.getType() == cocos2d::Value::Type::INTEGER) {
        json_t* jsonInt = json_integer(value.asInt());
        return jsonInt;
    } else if (value.getType() == cocos2d::Value::Type::DOUBLE) {
        json_t* jsonReal = json_real(value.asDouble());
        return jsonReal;
    } else if (value.getType() == cocos2d::Value::Type::FLOAT) {
        json_t* jsonString = json_real(value.asFloat());
        return jsonString;
    } else if (value.getType() == cocos2d::Value::Type::BOOLEAN) {
        json_t* jsonBoolean = json_boolean(value.asBool());
        return jsonBoolean;
    } else {
        CC_ASSERT(false);
        return NULL;
    }
}

