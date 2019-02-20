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

namespace ee {
namespace core {
template <class T>
class SharedPtrHandler {
public:
    static std::unique_ptr<SharedPtrHandler> create(se::Class* clazz) {
        return std::make_unique<SharedPtrHandler<T>>(clazz);
    }

    explicit SharedPtrHandler(se::Class* clazz)
        : clazz_(clazz) {}

    se::Class* getClass() {
        return clazz_;
    }

    std::shared_ptr<T> getValue(const se::Value& value) const {
        auto ptr = static_cast<T*>(value.toObject()->getPrivateData());
        auto&& iter = map_.find(ptr);
        if (iter == map_.cend()) {
            SE_REPORT_ERROR("Could not find shared_ptr.");
            return nullptr;
        }
        return iter->second;
    }

    void setValue(se::Value& value, const std::shared_ptr<T>& input) {
        if (input == nullptr) {
            value.setNull();
            return;
        }
        se::Object* obj = nullptr;
        auto ptr = input.get();
        auto iter = se::NativePtrToObjectMap::find(ptr);
        if (iter != se::NativePtrToObjectMap::end()) {
            obj = iter->second;
        } else {
            map_.emplace(ptr, input);
            obj = se::Object::createObjectWithClass(clazz_);
            obj->setPrivateData(ptr);
        }
        value.setObject(obj);
    }

    bool finalize(se::State& s) {
        auto ptr = static_cast<T*>(s.nativeThisObject());
        auto&& iter = map_.find(ptr);
        if (iter == map_.cend()) {
            SE_REPORT_ERROR("Could not find raw pointer.");
            delete ptr;
        } else {
            map_.erase(iter);
        }
        return true;
    }

private:
    se::Class* clazz_;

    /// Keeps strong reference to shared_ptr elements.
    std::unordered_map<T*, std::shared_ptr<T>> map_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_SHARED_PTR_HANDLER_HPP */
