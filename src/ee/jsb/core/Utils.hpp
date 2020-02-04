//
//  Utils.hpp
//  ee-x
//
//  Created by eps on 2/3/20.
//

#ifndef EE_X_UTILS_FOR_JSB_HPP
#define EE_X_UTILS_FOR_JSB_HPP

#include <functional>
#include <string>

#include "ee/jsb/JsbCoreFwd.hpp"

namespace ee {
namespace core {
void schedule(const std::function<void(float delta)>& callback, void* target,
              float interval, const std::string& key);

void runOnCocosThread(const std::function<void()>& callback);

bool getOrCreatePlainObject_r(const char* name, se::Object* parent,
                              se::Object** outObj);

se::Object* getPath(se::Object* parent, const std::string& path);
} // namespace core
} // namespace ee

#endif // EE_X_UTILS_FOR_JSB_HPP