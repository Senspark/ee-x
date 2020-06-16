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

#include <jni.h>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
class JniUtils final {
public:
    /// Retrieves the thread specified JNIEnv pointer corresponding to the
    /// current thread.
    static JNIEnv* getEnv();

    /// Checks and print any JNI exception.
    static void checkException();

    /// Converts Java string to C++ string.
    static std::string toString(jstring str);

    /// Converts C++ string to Java string.
    static std::unique_ptr<JniString> toJavaString(const std::string& str);

    static std::unique_ptr<JniString> toJavaString(const std::u16string& str);

    /// Retrieves the Java static method information.
    static std::unique_ptr<JniMethodInfo>
    getStaticMethodInfo(const char* className, const char* methodName,
                        const char* signature);

private:
    static JavaVM* getVm();
};
} // namespace core
} // namespace ee

#endif /* EE_X_JNI_UTILS_HPP */
