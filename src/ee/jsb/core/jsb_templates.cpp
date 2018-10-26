//
//  jsb_templates.cpp
//  ee-x
//
//  Created by Le Van Kiet on 10/24/18.
//

#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

template <>
se::Object* create_JSON_object(const std::pair<float, float>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <>
se::Object* create_JSON_object(const std::pair<int, int>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <>
se::Object*
create_JSON_object(const std::map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
se::Object*
create_JSON_object(const std::unordered_map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
se::Object* create_JSON_object(const int& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
se::Object* create_JSON_object(const std::string& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
se::Object* create_JSON_object(const bool& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
se::Object* create_JSON_object(const std::vector<std::string>& value) {
    auto&& jsonArray = nlohmann::json::array();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonArray.push_back(*it);
    }
    return se::Object::createJSONObject(jsonArray.dump());
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
void set_value(se::Value& value, std::int32_t input) {
    value.setInt32(input);
}

template <>
void set_value(se::Value& value, std::int64_t input) {
    auto temp = static_cast<std::int32_t>(input);
    value.setInt32(temp);
}

template <>
void set_value(se::Value& value, double input) {
    value.setFloat(input);
}

template <>
void set_value(se::Value& value, float input) {
    value.setFloat(input);
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
void set_value(se::Value& value, se::Object* obj) {
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, const std::pair<float, float>& input) {
    auto obj = create_JSON_object<std::pair<float, float>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, const std::pair<int, int>& input) {
    auto obj = create_JSON_object<std::pair<int, int>>(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value,
               const std::map<std::string, std::string>& input) {
    auto obj = create_JSON_object<std::map<std::string, std::string>>(input);
    value.setObject(obj);
}
} // namespace core
} // namespace ee
