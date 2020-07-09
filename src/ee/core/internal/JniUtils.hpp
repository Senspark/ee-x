//
//  EEJniUtils.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_X_JNI_UTILS_HPP
#define EE_X_JNI_UTILS_HPP

#include <string>
#include <thread>

#include "ee/core/internal/JniMethodInfo.hpp"

namespace ee {
namespace core {
class JniUtils final {
public:
    /// Sets the JavaVM pointer.
    static void setVm(JavaVM* vm);

    /// Retrieves the thread specified JNIEnv pointer corresponding to the
    /// current thread.
    static JNIEnv* getEnv();

    /// Converts Java string to C++ string.
    static std::string toString(jstring str);

    /// Converts C++ string to Java string.
    static std::unique_ptr<JniString> toJavaString(const std::string& str);

    static std::unique_ptr<JniString> toJavaString(const std::u16string& str);

    template <class... Args>
    static jboolean
    callStaticBooleanMethod(const char* className, const char* methodName,
                            const char* signature, Args&&... args) {
        auto method = getStaticMethodInfo(className, methodName, signature);
        return method->callStaticBooleanMethod(std::forward<Args>(args)...);
    }

    template <class... Args>
    static void callStaticVoidMethod(const char* className,
                                     const char* methodName,
                                     const char* signature, Args&&... args) {
        auto method = getStaticMethodInfo(className, methodName, signature);
        return method->callStaticVoidMethod(std::forward<Args>(args)...);
    }

    template <class... Args>
    static jobject
    callStaticObjectMethod(const char* className, const char* methodName,
                           const char* signature, Args&&... args) {
        auto method = getStaticMethodInfo(className, methodName, signature);
        return method->callStaticObjectMethod(std::forward<Args>(args)...);
    }

private:
    static JavaVM* getVm();

    /// Retrieves the Java static method information.
    static std::unique_ptr<JniMethodInfo>
    getStaticMethodInfo(const char* className, const char* methodName,
                        const char* signature);

    static JavaVM* vm_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_JNI_UTILS_HPP */
