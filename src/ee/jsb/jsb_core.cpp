//
//  jsb_core.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_core.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_log_level.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_metrics.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/core/jsb_utils.hpp"

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
