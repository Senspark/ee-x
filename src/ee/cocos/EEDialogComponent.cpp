//
//  EEDialogComponent.cpp
//  ee-library
//
//  Created by Zinge on 3/23/17.
//
//

#include "ee/cocos/EEDialogComponent.hpp"

#include <2d/CCNode.h>

namespace ee {
namespace cocos {
using Self = DialogComponent;

const std::string Self::DefaultName = "_ee_x_dialog_component_";

Self* Self::create() {
    auto result = new Self();
    result->init();
    result->autorelease();
    return result;
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
    setName(DefaultName);
    return true;
}

Self* Self::setPauseCallback(const PauseCallback& callback) {
    pauseCallback_ = callback;
    return this;
}

Self* Self::setResumeCallback(const ResumeCallback& callback) {
    resumeCallback_ = callback;
    return this;
}

void Self::resume(Dialog* dialog) {
    if (resumeCallback_) {
        resumeCallback_(dialog);
    }
}

void Self::pause(Dialog* dialog) {
    if (pauseCallback_) {
        pauseCallback_(dialog);
    }
}
} // namespace cocos
} // namespace ee
