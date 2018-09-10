//
//  EELazyGuard.hpp
//  ee-library
//
//  Created by Zinge on 6/20/17.
//
//

#ifndef EE_LIBRARY_LAZY_GUARD_HPP_
#define EE_LIBRARY_LAZY_GUARD_HPP_

#include <type_traits>

namespace cocos2d {
class Ref;
} // namespace cocos2d

namespace ee {
template <class ValueT> class LazyPtr final {
private:
    using Value = ValueT;
    using Pointer = Value*;

    static_assert(std::is_base_of<cocos2d::Ref, Value>::value,
                  "Value type must be derived from cocos2d::Ref");

public:
    LazyPtr(Pointer ptr)
        : ptr_(ptr) {
        ptr_->retain();
    }

    ~LazyPtr() {
        if (ptr_ != nullptr) {
            ptr_->autorelease();
            ptr_ = nullptr;
        }
    }

    LazyPtr(LazyPtr&& other)
        : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    operator Pointer() const { return ptr_; }
    Pointer operator->() const { return ptr_; }

    Pointer get() const { return ptr_; }

private:
    LazyPtr(const LazyPtr& other) = delete;
    LazyPtr& operator=(const LazyPtr& other) = delete;

    Pointer ptr_;
};

using LazyGuard = LazyPtr<cocos2d::Ref>;

template <class T> auto makeLazyGuard(T&& instance) {
    return LazyGuard(instance);
}

template <class T> auto makeLazyHolder(T&& instance) {
    using Tp = LazyPtr<std::remove_pointer_t<std::decay_t<T>>>;
    return Tp(std::forward<T>(instance));
}
} // namespace ee

#endif /* EE_LIBRARY_LAZY_GUARD_HPP_ */
