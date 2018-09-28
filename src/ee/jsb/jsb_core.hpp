//
//  jsb_core.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/16/18.
//

#ifndef JSB_CORE_HPP__
#define JSB_CORE_HPP__

#ifdef __cplusplus
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_log_level.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/core/jsb_utils.hpp"
#include "ee/jsb/core/jsb_metrics.hpp"

namespace ee {
namespace core {
bool register_all_core_manual(se::Object* object) {
    register_log_level_manual(object);
    register_logger_manual(object);
    register_utils_manual(object);
    register_metrics_manual(object);
    return true;
}
} // namespace core
} // namespace ee

#endif // __cplusplus
#endif // JSB_CORE_HPP
