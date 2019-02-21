//
//  SharedPtrHandler.hpp
//  Pods
//
//  Created by eps on 2/20/19.
//

#ifndef EE_X_SHARED_PTR_HANDLER_HPP
#define EE_X_SHARED_PTR_HANDLER_HPP

#include <memory>
#include <unordered_map>

#include <cocos/scripting/js-bindings/jswrapper/SeApi.h>

#include "ee/jsb/core/jsb_utils.hpp"

namespace ee {
namespace core {
template <class T>
class SharedPtrHandler {
public:
    static std::unique_ptr<SharedPtrHandler> create(se::Class* clazz) {
        return std::make_unique<SharedPtrHandler>(clazz);
    }

    explicit SharedPtrHandler(se::Class* clazz)
        : clazz_(clazz) {
        schedule(
            [this](float delta) { //
                garbageCollect();
            },
            this, 0.0f, "shared_ptr_handler");
    }

    se::Class* getClass() {
        return clazz_;
    }

    std::shared_ptr<T> getValue(const se::Value& value) const {
        auto ptr = static_cast<T*>(value.toObject()->getPrivateData());
        auto&& iter = pointers_.find(ptr);
        if (iter == pointers_.cend()) {
            SE_REPORT_ERROR("Could not find element for ptr = %p", ptr);
            return nullptr;
        }
        return iter->second;
    }

    void setValue(se::Value& value, const std::shared_ptr<T>& input) {
        if (input == nullptr) {
            value.setNull();
            return;
        }
        auto ptr = input.get();
        auto iter = se::NativePtrToObjectMap::find(ptr);
        if (iter != se::NativePtrToObjectMap::end()) {
            auto&& obj = iter->second;
            value.setObject(obj);
        } else {
            auto&& obj = se::Object::createObjectWithClass(clazz_);
            obj->setPrivateData(ptr);
            value.setObject(obj, true);
            obj->root();
            pointers_.emplace(ptr, input);
            objects_.emplace(ptr, obj);
        }
    }

    bool finalize(se::State& state) {
        auto ptr = static_cast<T*>(state.nativeThisObject());
        auto&& iter = pointers_.find(ptr);
        if (iter == pointers_.cend()) {
            SE_REPORT_ERROR("Could not find shared_ptr for ptr = %p.", ptr);
            return false;
        }
        pointers_.erase(iter);
        return true;
    }

private:
    /// Performs a garbage collection.
    void garbageCollect() {
        for (auto iter = objects_.begin(); iter != objects_.end();) {
            auto&& [ptr, obj] = *iter;
            auto&& found = se::NativePtrToObjectMap::find(ptr);
            assert(found != se::NativePtrToObjectMap::end());
            assert(found->second == obj);
            assert(obj->isRooted());
            auto&& sharedPtr = pointers_.find(ptr)->second;
            auto&& useCount = sharedPtr.use_count();
            if (useCount == 1) {
                obj->unroot();
                iter = objects_.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    se::Class* clazz_;

    /// Keeps strong reference to shared_ptr elements.
    std::unordered_map<T*, std::shared_ptr<T>> pointers_;

    /// Keeps weak reference to se::Object elements;
    std::unordered_map<T*, se::Object*> objects_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_SHARED_PTR_HANDLER_HPP */
