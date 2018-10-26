//
//  EEMultilingualDelegate.cpp
//  ee-library
//
//  Created by Zinge on 7/31/17.
//
//

#include "EEMultilingualDelegate.hpp"

#include <2d/CCNode.h>
#include <base/CCValue.h>

namespace ee {
using Self = MultilingualDelegate;

void Self::onNodeLoaded(cocos2d::Node* node,
                        cocosbuilder::NodeLoader* nodeLoader) {
    CC_ASSERT(key_);
}

bool Self::onAssignCCBMemberVariable(cocos2d::Ref* target,
                                     const char* memberVariableName,
                                     cocos2d::Node* node) {
    return false;
}

bool Self::onAssignCCBCustomProperty(cocos2d::Ref* target,
                                     const char* memberVariableName,
                                     const cocos2d::Value& value) {
    if (dynamic_cast<Self*>(target) == this) {
        std::string name = memberVariableName;
        if (name == "multilingual_key") {
            CC_ASSERT(value.getType() == cocos2d::Value::Type::STRING);
            auto key = value.asString();
            setKey(key);
        }
    }
    return false;
}
} // namespace ee
