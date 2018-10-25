//
//  jsb_common.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#include "ee/jsb/core/jsb_core_common.hpp"

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"

namespace ee {
namespace core {

bool getOrCreatePlainObject_r(const char* name, se::Object* parent,
                              se::Object** outObj) {
    assert(parent != nullptr);
    assert(outObj != nullptr);
    se::Value tmp;

    if (parent->getProperty(name, &tmp) && tmp.isObject()) {
        *outObj = tmp.toObject();
        (*outObj)->incRef();
    } else {
        *outObj = se::Object::createPlainObject();
        parent->setProperty(name, se::Value(*outObj));
    }

    return true;
}

} // namespace core
} // namespace ee
