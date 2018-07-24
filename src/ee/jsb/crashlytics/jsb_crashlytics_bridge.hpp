//
//  jsb_crashlytics_bridge.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/24/18.
//

#ifndef JSB_CRASHLYTICS_BRIDGE_HPP__
#define JSB_CRASHLYTICS_BRIDGE_HPP__

#pragma once

namespace se {
class Object;
} // namespace se

namespace ee {
namespace crashlytics {

bool register_crashlytics_manual(se::Object* object);

} // namespace crashlytics
} // namespace ee

#endif //JSB_CRASHLYTICS_BRIDGE_HPP__
