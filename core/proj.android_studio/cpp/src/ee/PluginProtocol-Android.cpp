//
//  __ParseBridge-Android.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#include "ee/PluginProtocol.hpp"
#include "ee/PluginManager.hpp"
#include "ee/JniUtils.hpp"
#include "ee/JniMethodInfo.hpp"
#include "ee/JniString.hpp"
#include "ee/Logger.hpp"

extern "C" {
JNIEXPORT void JNICALL
Java_com_ee_core_PluginProtocol__1sendCppMessage0(JNIEnv* env, jobject thiz,
                                                  jstring arg0, jstring arg1,
                                                  jint arg2) {
    using namespace ee::core;

    auto pluginName = JniUtils::toString(arg0);
    auto json = JniUtils::toString(arg1);
    auto callbackId = static_cast<int>(arg2);

    PluginManager::onNativeCallback(pluginName, json, callbackId);
}

JNIEXPORT void JNICALL
Java_com_ee_core_PluginProtocol__1sendCppMessage1(JNIEnv* env, jobject thiz,
                                                  jstring arg0, jstring arg1,
                                                  jstring arg2) {
    using namespace ee::core;

    auto pluginName = JniUtils::toString(arg0);
    auto json = JniUtils::toString(arg1);
    auto tag = JniUtils::toString(arg2);

    PluginManager::onNativeCallback(pluginName, json, tag);
}
} // extern "C"

namespace ee {
namespace core {
std::string PluginProtocol::callNative1(const std::string& methodName_,
                                        const optional<std::string>& json_,
                                        const optional<int>& callbackId) const {
    auto methodInfo = JniUtils::getStaticMethodInfo(
        "com/ee/core/PluginManager", "callNative", "("
                                                   "Ljava/lang/String;"
                                                   "Ljava/lang/String;"
                                                   "Ljava/lang/String;"
                                                   "I"
                                                   ")"
                                                   "Ljava/lang/String;");

    if (methodInfo != nullptr) {
        auto pluginName = JniUtils::toJavaString(pluginName_.c_str());
        auto methodName = JniUtils::toJavaString(methodName_.c_str());
        auto json =
            JniUtils::toJavaString(json_ ? json_.value().c_str() : nullptr);

        jobject response = methodInfo->getEnv()->CallStaticObjectMethod(
            methodInfo->getClass(), methodInfo->getMethodId(),
            pluginName->get(), methodName->get(), json->get(),
            callbackId.value_or(-1));

        jstring msg = static_cast<jstring>(response);

        auto result = JniUtils::toString(msg);

        methodInfo->getEnv()->DeleteLocalRef(response);

        return result;
    }

    throw std::runtime_error("Method not found!");
}
} // namespace core
} // namespace ee