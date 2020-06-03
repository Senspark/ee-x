//
//  SharedPtrManager.hpp
//  Pods
//
//  Created by eps on 2/20/19.
//

#ifndef EE_X_SHARED_PTR_UTILS_HPP
#define EE_X_SHARED_PTR_UTILS_HPP

#ifdef __cplusplus

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace ee {
namespace core {
template <class T>
using SharedPtrDeleter = std::function<void(T* instance)>;

namespace internal {
template <class T>
SharedPtrDeleter<T>& getDeleter() {
    static SharedPtrDeleter<T> deleter = [](T* instance) {
        // Default deleter.
        delete instance;
    };
    return deleter;
}
} // namespace internal

/// Creates a shared_ptr with a custom deleter.
template <class T, class... Args>
std::shared_ptr<T> makeShared(Args&&... args) {
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...),
                              [](T* instance) {
                                  auto&& deleter = internal::getDeleter<T>();
                                  deleter(instance);
                              });
}

// Registers shared_ptr deleter for the specified class.
template <class T>
void registerDeleter(const SharedPtrDeleter<T>& deleter) {
    internal::getDeleter<T>() = deleter;
}
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_SHARED_PTR_UTILS_HPP */
