//
//  EEJniMethodInfo.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_CORE_JNI_METHOD_INFO_HPP_
#define EE_CORE_JNI_METHOD_INFO_HPP_

#include <memory>

#include <jni.h>

namespace ee {
namespace core {
class JniMethodInfo final {
public:
    static std::unique_ptr<JniMethodInfo> create(JNIEnv* env, jclass clazz,
                                                 jmethodID methodId);
    ~JniMethodInfo();

    jclass getClass() const noexcept;

    jmethodID getMethodId() const noexcept;

    JNIEnv* getEnv() const noexcept;

private:
    explicit JniMethodInfo(JNIEnv* env, jclass clazz, jmethodID methodId);

    JNIEnv* env_;
    jclass clazz_;
    jmethodID methodId_;
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_JNI_METHOD_INFO_HPP_ */
