//
//  jsb_log_level.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef JSB_LOG_LEVEL_HPP__
#define JSB_LOG_LEVEL_HPP__

#include "ee/jsb/core/jsb_templates.hpp"

namespace se {
class Object;
} // namespace se

namespace ee {
namespace core {

class LogLevel;

template <>
const LogLevel& get_value<const LogLevel&>(const se::Value& value);

template <>
void set_value<const LogLevel&>(se::Value& value, const LogLevel& input);

bool register_log_level_manual(se::Object* object);

} // namespace core
} // namespace ee

#endif /* JSB_LOG_LEVEL_HPP__ */
