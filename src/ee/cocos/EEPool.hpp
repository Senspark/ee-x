//
//  EEPool.hpp
//  ee-library
//
//  Created by Zinge on 9/28/16.
//
//

#ifndef EE_LIBRARY_POOL_HPP_
#define EE_LIBRARY_POOL_HPP_

#include <functional>
#include <queue>
#include <vector>

#include <base/CCRefPtr.h>

namespace ee {
/// Be careful with objects that are not added to scene.
/// They will be leaked (issue #14050).
///
/// Also when using CocosBuilder, CCBAnimationManager will also be leaked
/// because there is a circular reference.
/// Workaround: loaded_node->setUserObject(nullptr) (must do for all children)
/// to remove the CCBAnimationManager.
template <class T> class Pool final {
public:
    using value_type = T;
    using pointer = T*;
    using reference = cocos2d::RefPtr<value_type>;
    using const_pointer = const pointer;
    using constructor = std::function<pointer()>;
    using destructor = std::function<void(pointer)>;
    using predicate = std::function<bool(const_pointer instance)>;
    using size_type = std::size_t;

    explicit Pool(const constructor& ctor, const destructor& dtor = nullptr)
        : Pool(ctor, dtor, [](const_pointer instance) {
            return instance->getReferenceCount() == 1;
        }) {}

    explicit Pool(const constructor& ctor, const destructor& dtor,
                  const predicate& pred)
        : ctor_(ctor)
        , dtor_(dtor)
        , pred_(pred) {}

    size_type size() const { return unused_instances_.size(); }

    size_type capacity() const {
        return using_instances_.size() + unused_instances_.size();
    }

    void reserve(size_type n) {
        auto cap = capacity();
        if (n > cap) {
            auto difference = n - cap;
            for (size_type i = 0; i < difference; ++i) {
                unused_instances_.emplace(instantiate());
            }
        }
    }

    pointer pop() {
        check_for_unused_instances();
        if (unused_instances_.empty()) {
            unused_instances_.emplace(instantiate());
        }

        auto instance = unused_instances_.front();
        using_instances_.emplace_back(instance);
        unused_instances_.pop();
        return instance;
    }

    void push(pointer instance, bool destroy = true) {
        if (destroy && dtor_) {
            dtor_(instance);
        }
        unused_instances_.emplace(instance);
    }

protected:
    pointer instantiate() const { return ctor_(); }

    void check_for_unused_instances() {
        auto iter =
            std::remove_if(using_instances_.begin(), using_instances_.end(),
                           [this](const_pointer instance) {
                               if (pred_(instance)) {
                                   push(instance);
                                   return true;
                               }
                               return false;
                           });
        using_instances_.erase(iter, using_instances_.cend());
    }

    std::queue<reference> unused_instances_;
    std::vector<reference> using_instances_;

    constructor ctor_;
    destructor dtor_;
    predicate pred_;
};
} // namespace ee

#endif /* EE_LIBRARY_POOL_HPP_ */
