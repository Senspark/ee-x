//
//  PluginProtocol.cpp
//  ee-x
//
//  Created by Zinge on 7/1/16.
//
//

#include <cassert>

#include "ee/PluginProtocol.hpp"
#include "ee/PluginManager.hpp"

namespace ee {
namespace core {
PluginProtocol::PluginProtocol(const std::string& pluginName)
    : pluginName_(pluginName) {
    bool inserted = PluginManager::addPlugin(pluginName_, this);
    assert(inserted);
}

PluginProtocol::~PluginProtocol() { PluginManager::removePlugin(pluginName_); }

std::string PluginProtocol::callNative(const std::string& methodName,
                                       const NativeCallback& callback) {
    return callNative0(methodName, nullopt, callback);
}

std::string PluginProtocol::callNative(const std::string& methodName,
                                       const std::string& json,
                                       const NativeCallback& callback) {
    return callNative0(methodName, json, callback);
}

std::string PluginProtocol::callNative(const std::string& methodName) const {
    return callNative1(methodName, nullopt, nullopt);
}

std::string PluginProtocol::callNative(const std::string& methodName,
                                       const std::string& json) const {
    return callNative1(methodName, json, nullopt);
}

std::string PluginProtocol::callNative0(const std::string& methodName,
                                        const optional<std::string>& json,
                                        const NativeCallback& callback) {
    int callbackId = callbackIdCounter_++;
    pool_.emplace(callbackId, callback);
    return callNative1(methodName, json, callbackId);
}

bool PluginProtocol::addCallback(const std::string& tag,
                                 const NativeCallback& callback) {
    auto iter = persistPool_.emplace(tag, callback);
    return iter.second;
}

bool PluginProtocol::removeCallback(const std::string& tag) {
    if (persistPool_.count(tag)) {
        persistPool_.erase(tag);
        return true;
    }
    return false;
}
} // namespace core
} // namespace ee