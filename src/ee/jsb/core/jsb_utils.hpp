//
//  jsbUtils.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#ifndef JSB_EE_UTILS_HPP__
#define JSB_EE_UTILS_HPP__

#include <functional>

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {
void schedule(const std::function<void(float delta)>& callback, void* target,
              float interval, const std::string& key);
void runOnCocosThread(const std::function<void()>& callback);
bool register_utils_manual(se::Object* object);
} // namespace core
} // namespace ee

#endif /* JSB_EE_UTILS_HPP__ */
