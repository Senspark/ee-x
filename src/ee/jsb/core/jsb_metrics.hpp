//
//  jsb_metrics.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/26/18.
//

#ifndef jsb_metrics_hpp
#define jsb_metrics_hpp

#include "ee/jsb/core/jsb_templates.hpp"

namespace se {
class Object;
} // namespace se

namespace ee {
namespace core {

bool register_metrics_manual(se::Object* object);

} // namespace core
} // namespace ee

#endif /* jsb_metrics_hpp */
