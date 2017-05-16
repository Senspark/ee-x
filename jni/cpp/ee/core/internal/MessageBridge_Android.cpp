//
//  __ParseBridge-Android.cpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#include "ee/core/internal/MessageBridge.hpp"
#include "ee/core/JniUtils.hpp"
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/Logger.hpp"

namespace ee {
namespace core {
namespace {
extern "C" {
JNIEXPORT jstring JNICALL
Java_com_ee_core_internal_MessageBridge_callCpp(JNIEnv* env, jobject instance,
                                                jstring tag_, jstring msg_) {
    const char* tag = env->GetStringUTFChars(tag_, nullptr);
    const char* msg = env->GetStringUTFChars(msg_, nullptr);

    auto result = MessageBridge::getInstance().call(tag, msg);

    env->ReleaseStringUTFChars(tag_, tag);
    env->ReleaseStringUTFChars(msg_, msg);

    return env->NewStringUTF(result.c_str());
}
} // extern "C"
} // namespace

std::string MessageBridge::call(const std::string& tag,
                                const std::string& msg) {
    auto methodInfo = JniUtils::getStaticMethodInfo(
        "com/ee/core/internal/MessageBridge", "staticCall",
        "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

    if (methodInfo == nullptr) {
        throw std::runtime_error("Method not found!");
    }

    auto tag_java = JniUtils::toJavaString(tag.c_str());
    auto msg_java = JniUtils::toJavaString(msg.c_str());

    jobject response = methodInfo->getEnv()->CallStaticObjectMethod(
        methodInfo->getClass(), methodInfo->getMethodId(), tag_java->get(),
        msg_java->get());

    jstring response_java = static_cast<jstring>(response);
    auto result = JniUtils::toString(response_java);

    methodInfo->getEnv()->DeleteLocalRef(response);

    return result;
}
} // namespace core
} // namespace ee
