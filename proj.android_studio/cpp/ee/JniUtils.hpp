//
//  EEJniUtils.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_CORE_JNI_UTILS_HPP_
#define EE_CORE_JNI_UTILS_HPP_

#include <string>
#include <thread>

#include <jni.h>

namespace ee {
class JniMethodInfo;
class JniString;

class JniUtils final {
public:
    /// Assigns the JavaVM pointer to retrieve JNIEnv pointer later.
    ///
    /// Calls this in main.cpp.
    static void setVm(JavaVM* vm);

    /// Marks the Cocos thread.
    ///
    /// Calls this in AppDelegate.cpp.
    static void markCocosThread() noexcept;

    /// Retrieves the thread specified JNIEnv pointer corresponding to the
    /// current thread.
    static JNIEnv* getEnv();

    /// Checks and print any JNI exception.
    static void checkException();

    /// Converts Java string to C++ string.
    static std::string toString(jstring str);

    /// Converts C++ string to Java string.
    static std::unique_ptr<JniString> toJavaString(const char* str);

    /// Retrieves the Java static method information.
    static std::unique_ptr<JniMethodInfo>
    getStaticMethodInfo(const char* className, const char* methodName,
                        const char* signature);

private:
    /// Caches the JNIEnv pointer for the current thread.
    static JNIEnv* cacheEnv();

    /// Destructor for pthread.
    static void detachCurrentThread(void*);

    /// Marked Cocos thread.
    static bool isCocosThreadMarked_;
    static std::thread::id cocosThreadId_;

    /// Assigned JavaVM pointer.
    static JavaVM* vm_;
};
} // namespace ee

#endif /* EE_CORE_JNI_UTILS_HPP_ */
