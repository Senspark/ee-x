//
//  jsb_logger.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#ifndef JSB_LOGGER_HPP__
#define JSB_LOGGER_HPP__

#include "ee/jsb/core/jsb_templates.hpp"

namespace se {
class Object;
} // namespace se

namespace ee {
namespace core {

class Logger;

template <>
const Logger& get_value(const se::Value& value);

template <>
void set_value<Logger&>(se::Value& value, Logger& input);

bool register_logger_manual(se::Object* object);

} // namespace core
} // namespace ee

#endif /* JSB_LOGGER_HPP__ */
