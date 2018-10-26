//
//  EEPreferences.cpp
//  ee-library
//
//  Created by Zinge on 2/27/17.
//
//

#include "EEPreferences.hpp"
#include "EEDataHandler.hpp"

#include <base/CCDirector.h>
#include <base/CCScheduler.h>
#include <platform/CCFileUtils.h>

#include <json/document.h>
#include <json/stringbuffer.h>
#include <json/writer.h>

NS_EE_BEGIN
namespace {
void writeData(const DataStorage& data, const std::string& filePath) {
    auto fileUtils = cocos2d::FileUtils::getInstance();
    rapidjson::Document document;
    document.SetObject();
    for (auto&& elt : data) {
        document.AddMember(rapidjson::StringRef(elt.first.c_str()),
                           rapidjson::StringRef(elt.second.c_str()),
                           document.GetAllocator());
    }
    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    fileUtils->writeStringToFile(buffer.GetString(), filePath);
}

DataStorage readData(const std::string& filePath) {
    auto fileUtils = cocos2d::FileUtils::getInstance();
    auto content = fileUtils->getStringFromFile(filePath);
    DataStorage data;
    if (not content.empty()) {
        rapidjson::Document document;
        document.Parse(content.c_str());
        if (not document.HasParseError()) {
            for (auto iter = document.MemberBegin();
                 iter != document.MemberEnd(); ++iter) {
                data[iter->name.GetString()] = iter->value.GetString();
            }
        }
    }
    return data;
}
} // namespace

void Preferences::initializeWithFilename(const std::string& filename) {
    if (isInitialized_) {
        return;
    }
    handler_ = std::make_unique<ee::DataHandler>(0);

    handler_->setCallback(
        std::bind(&Self::onSaving, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));

    handler_->setCallback(
        std::bind(&Self::onLoading, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));

    handler_->setCallback(
        [](int dataId, const std::string& key) { CC_ASSERT(false); });

    filePath_ = cocos2d::FileUtils::getInstance()->getWritablePath() + filename;
    persistedData_ = readData(filePath_);
    isInitialized_ = true;
}

Preferences::Preferences() {
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        std::bind(&Self::updateData, this), this, 0.0f, CC_REPEAT_FOREVER, 0.0f,
        false, "update__");
}

Preferences::~Preferences() {
    cocos2d::Director::getInstance()->getScheduler()->unschedule("update__",
                                                                 this);
}

std::string Preferences::encrypt(const std::string& value) const {
    return encrypt("", value);
}

std::string Preferences::encrypt(const std::string& key,
                                 const std::string& value) const {
    auto hash = calculateHash(key + value);
    return value + "_" + hash;
}

bool Preferences::decrypt(const std::string& encryptedValue,
                          std::string& result) const {
    return decrypt(encryptedValue, "", result);
}

bool Preferences::decrypt(const std::string& encryptedValue,
                          const std::string& key, std::string& result) const {
    auto dashIndex = encryptedValue.rfind('_');
    if (dashIndex == std::string::npos) {
        return false;
    }
    auto storedHash = encryptedValue.substr(dashIndex + 1);
    auto storedValue = encryptedValue.substr(0, dashIndex);
    auto expectedHash = calculateHash(key + storedValue);
    if (storedHash != expectedHash) {
        return false;
    }
    result = storedValue;
    return true;
}

void Preferences::save(const std::string& key, const std::string& value,
                       Encryption mode) {
    if (mode == Encryption::Value) {
        return save(key, encrypt(value), Encryption::None);
    }
    if (mode == Encryption::KeyValue) {
        return save(key, encrypt(key, value), Encryption::None);
    }
    dirtyData_[key] = value;
}

bool Preferences::load(const std::string& key, std::string& value,
                       Encryption mode) {

    if (mode == Encryption::Value || mode == Encryption::KeyValue) {
        if (not load(key, value, Encryption::None)) {
            return false;
        }
        std::string decryptedValue;
        if (mode == Encryption::Value && decrypt(value, decryptedValue)) {
            value = decryptedValue;
            return true;
        }
        if (mode == Encryption::KeyValue &&
            decrypt(value, key, decryptedValue)) {
            value = decryptedValue;
            return true;
        }
        return false;
    }
    if (dirtyData_.count(key) > 0) {
        value = dirtyData_[key];
        return true;
    }
    if (persistedData_.count(key) > 0) {
        value = persistedData_[key];
        return true;
    }
    return false;
}

void Preferences::forceLoadData() {
    merge();
    auto data = readData(filePath_);
    merge(data);
    writeData(persistedData_, filePath_);
}

void Preferences::updateData() {
    if (merge()) {
        writeData(persistedData_, filePath_);
    }
}

bool Preferences::merge() {
    return merge(dirtyData_);
}

bool Preferences::merge(DataStorage& data) {
    if (data.empty()) {
        return false;
    }
    for (auto&& iter : data) {
        persistedData_[iter.first] = iter.second;
    }
    data.clear();
    return true;
}
NS_EE_END
