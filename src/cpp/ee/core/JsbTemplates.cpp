//
//  jsb_templates.cpp
//  ee-x
//
//  Created by Le Van Kiet on 10/24/18.
//

#include "ee/core/JsbTemplates.hpp"

#ifdef EE_X_COCOS_JS
#include <string>
#include <unordered_map>
#include <vector>

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace core {
template <>
se::HandleObject create_JSON_object(const std::pair<float, float>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::HandleObject(se::Object::createJSONObject(jsonArray.dump()));
}

template <>
se::HandleObject create_JSON_object(const std::pair<int, int>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::HandleObject(se::Object::createJSONObject(jsonArray.dump()));
}

template <>
se::HandleObject
create_JSON_object(const std::map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::HandleObject(se::Object::createJSONObject(jsonObj.dump()));
}

template <>
se::HandleObject
create_JSON_object(const std::unordered_map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::HandleObject(se::Object::createJSONObject(jsonObj.dump()));
}

template <>
se::HandleObject create_JSON_object(const int& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::HandleObject(se::Object::createJSONObject(jsonObj.dump()));
}

template <>
se::HandleObject create_JSON_object(const std::string& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::HandleObject(se::Object::createJSONObject(jsonObj.dump()));
}

template <>
se::HandleObject create_JSON_object(const bool& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::HandleObject(se::Object::createJSONObject(jsonObj.dump()));
}

template <>
se::HandleObject create_JSON_object(const std::vector<std::string>& value) {
    auto&& jsonArray = nlohmann::json::array();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonArray.push_back(*it);
    }
    return se::HandleObject(se::Object::createJSONObject(jsonArray.dump()));
}

template <>
std::unordered_map<std::string, std::string>
from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::unordered_map<std::string, std::string> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);
        ret[key] = value.toString();
    }
    return ret;
}

template <>
std::unordered_map<std::string, cocos2d::Value>
from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::unordered_map<std::string, cocos2d::Value> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);

        if (value.isNumber()) {
            ret[key] = cocos2d::Value(value.toNumber());
        } else if (value.isString()) {
            ret[key] = cocos2d::Value(value.toString());
        } else if (value.isBoolean()) {
            ret[key] = cocos2d::Value(value.toBoolean());
        } else {
            ret[key] = cocos2d::Value::Null;
        }
    }
    return ret;
}

template <>
std::map<std::string, std::string> from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::map<std::string, std::string> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);
        ret[key] = value.toString();
    }
    return ret;
}

template <>
std::vector<std::string> from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> ret;
    std::uint32_t arraySize;

    jsonObj->getArrayLength(&arraySize);
    for (int i = 0; i < arraySize; i++) {
        se::Value value;
        jsonObj->getArrayElement(i, &value);
        ret.push_back(value.toString());
    }

    return ret;
}

template <>
bool get_value(const se::Value& value) {
    return value.toBoolean();
}

template <>
std::int32_t get_value(const se::Value& value) {
    return value.toInt32();
}

template <>
std::int64_t get_value(const se::Value& value) {
    return static_cast<std::int64_t>(value.toLong());
}

template <>
float get_value(const se::Value& value) {
    return value.toFloat();
}

template <>
double get_value(const se::Value& value) {
    return value.toNumber();
}

template <>
std::string get_value(const se::Value& value) {
    return value.toString();
}

template <>
const std::string& get_value(const se::Value& value) {
    return value.toString();
}

template <>
std::unordered_map<std::string, std::string> get_value(const se::Value& value) {
    return from_JSON_object<std::unordered_map<std::string, std::string>>(
        value.toObject());
}

template <>
std::map<std::string, std::string> get_value(const se::Value& value) {
    return from_JSON_object<std::map<std::string, std::string>>(
        value.toObject());
}

template <>
std::unordered_map<std::string, cocos2d::Value>
get_value(const se::Value& value) {
    return from_JSON_object<std::unordered_map<std::string, cocos2d::Value>>(
        value.toObject());
}

template <>
cocos2d::Value get_value(const se::Value& value) {
    if (value.isNumber()) {
        return cocos2d::Value(value.toNumber());
    } else if (value.isString()) {
        return cocos2d::Value(value.toString());
    } else if (value.isBoolean()) {
        return cocos2d::Value(value.toBoolean());
    } else {
        return cocos2d::Value::Null;
    }
}

template <>
std::vector<std::string> get_value(const se::Value& value) {
    return from_JSON_object<std::vector<std::string>>(value.toObject());
}

template <>
void set_value(se::Value& value, std::int32_t input) {
    value.setInt32(input);
}

template <>
void set_value(se::Value& value, std::int32_t& input) {
    value.setInt32(input);
}

template <>
void set_value(se::Value& value, std::int64_t& input) {
    value.setInt32(static_cast<double>(input));
}

template <>
void set_value(se::Value& value, std::uint64_t& input) {
    value.setNumber(static_cast<double>(input));
}

template <>
void set_value(se::Value& value, double& input) {
    value.setFloat(input);
}

template <>
void set_value(se::Value& value, float& input) {
    value.setFloat(input);
}

template <>
void set_value(se::Value& value, std::string input) {
    value.setString(std::move(input));
}

template <>
void set_value(se::Value& value, std::string& input) {
    value.setString(input);
}

template <>
void set_value(se::Value& value, const std::string& input) {
    value.setString(input);
}

template <>
void set_value(se::Value& value, const char* input) {
    value.setString(input);
}

template <>
void set_value(se::Value& value, bool input) {
    value.setBoolean(input);
}

template <>
void set_value(se::Value& value, bool& input) {
    value.setBoolean(input);
}

template <>
void set_value(se::Value& value, se::Object* obj) {
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::pair<float, float> input) {
    auto obj = create_JSON_object(std::move(input));
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::pair<float, float>& input) {
    auto obj = create_JSON_object<std::pair<float, float>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, const std::pair<float, float>& input) {
    auto obj = create_JSON_object<std::pair<float, float>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::pair<int, int> input) {
    auto obj = create_JSON_object(std::move(input));
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::pair<int, int>& input) {
    auto obj = create_JSON_object<std::pair<int, int>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, const std::pair<int, int>& input) {
    auto obj = create_JSON_object<std::pair<int, int>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::map<std::string, std::string>& input) {
    auto obj = create_JSON_object<std::map<std::string, std::string>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value,
               const std::map<std::string, std::string>& input) {
    auto obj = create_JSON_object<std::map<std::string, std::string>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value,
               std::unordered_map<std::string, std::string> input) {
    auto obj = create_JSON_object(std::move(input));
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, const std::vector<std::string>& input) {
    auto obj = create_JSON_object<std::vector<std::string>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, std::vector<std::string> input) {
    auto obj = create_JSON_object(std::move(input));
    value.setObject(obj);
}

namespace internal {
void callFunction(const se::Value& jsThis, const se::Value& jsFunc,
                  const se::ValueArray& args) {
    se::Object* thisObj = jsThis.isObject() ? jsThis.toObject() : nullptr;
    auto funcObj = jsFunc.toObject();
    auto succeed = funcObj->call(args, thisObj);
    if (!succeed) {
        se::ScriptEngine::getInstance()->clearException();
    }
}
} // namespace internal
} // namespace core
} // namespace ee

#endif // EE_X_COCOS_JS