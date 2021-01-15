#include "ee/core/internal/MessageBridgeUtils.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

namespace ee {
namespace core {
using Self = MessageBridgeUtils;

#ifdef EE_X_ANDROID
std::string Self::call(const std::string& tag, const std::string& message) {
    jobject response = JniUtils::callStaticObjectMethod(
        "com/ee/internal/MessageBridgeKt", //
        "ee_staticCall",                   //
        "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
        JniUtils::toJavaString(tag)->get(),
        JniUtils::toJavaString(message)->get());

    jstring response_java = static_cast<jstring>(response);
    auto result = JniUtils::toString(response_java);

    JniUtils::getEnv()->DeleteLocalRef(response);
    return result;
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
char* ee_staticCall(const char* tag, const char* message);
} // extern "C"

std::string Self::call(const std::string& tag, const std::string& message) {
    auto response_swift = ee_staticCall(tag.c_str(), message.c_str());
    if (response_swift == nullptr) {
        return "";
    }
    auto result = std::string(response_swift);
    std::free(response_swift);
    return result;
}
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
} // namespace core
} // namespace ee