//
//  jsb_ironsource.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/JsbIronSource.hpp"

#include "ee/jsb/iron_source/JsbIronSourceBridge.hpp"

namespace ee {
namespace iron_source {
bool registerJsb(se::Object* global) {
    registerJsbBridge(global);
    return true;
}
} // namespace iron_source
} // namespace ee
