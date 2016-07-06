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
namespace core {
class JniMethodInfo;
class JniString;

class JniUtils final {
public:
    static void setVm(JavaVM* vm) noexcept;
    
    static void initialize();

    static JNIEnv* getEnv();

    static void checkException();

    static std::string toString(jstring str);

    static std::unique_ptr<JniString> toJavaString(const char* str);

    static std::unique_ptr<JniMethodInfo>
    getStaticMethodInfo(const char* className, const char* methodName,
                        const char* signature);

private:
    static void cacheEnv();
    
    static std::thread::id threadId_;

    static JNIEnv* env_;
    static JavaVM* vm_;
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_JNI_UTILS_HPP_ */
