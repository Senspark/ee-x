//
//  JniString.hpp
//  ee-core
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_JNI_STRING_HPP
#define EE_X_JNI_STRING_HPP

#include <memory>
#include <string>

#include <jni.h>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
/// RAII class for encapsulating jstring.
class JniString {
public:
    static std::unique_ptr<JniString> create(JNIEnv* env,
                                             const std::u16string& str);

    ~JniString();

    jstring get() const noexcept;

private:
    explicit JniString(JNIEnv* env, const std::u16string& str);

    /// Weak pointer to the JNIEnv.
    JNIEnv* env_;

    /// Strong pointer the jstring.
    jstring javaString_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_JNI_STRING_HPP */
