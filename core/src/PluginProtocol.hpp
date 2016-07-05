//
//  PluginProtocol.hpp
//  ee-x
//
//  Created by Zinge on 7/1/16.
//
//

#ifndef EE_CORE_PLUGIN_PROTOCOL_HPP_
#define EE_CORE_PLUGIN_PROTOCOL_HPP_

#include <functional>
#include <string>
#include <unordered_map>

#include "ExperimentalOptional.hpp"

namespace ee {
namespace core {
class PluginProtocol {
public:
    using NativeCallback = std::function<void(const std::string& json)>;

    explicit PluginProtocol(const std::string& pluginName);

    virtual ~PluginProtocol();

    std::string callNative(const std::string& methodName,
                           const NativeCallback& callback);

    std::string callNative(const std::string& methodName,
                           const std::string& json,
                           const NativeCallback& callback);

    std::string callNative(const std::string& methodName) const;

    std::string callNative(const std::string& methodName,
                           const std::string& json) const;

    bool addCallback(const std::string& tag, const NativeCallback& callback);

    bool removeCallback(const std::string& tag);

private:
    friend class PluginManager;

    std::string callNative0(const std::string& methodName,
                            const optional<std::string>& json,
                            const NativeCallback& callback);

    std::string callNative1(const std::string& methodName,
                            const optional<std::string>& json,
                            const optional<int>& callbackId) const;

    std::string pluginName_;

    int callbackIdCounter_;
    std::unordered_map<int, NativeCallback> pool_;
    
    std::unordered_map<std::string, NativeCallback> persistPool_;
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_PLUGIN_PROTOCOL_HPP_ */
