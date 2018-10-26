//
//  EEDialogComponent.cpp
//  ee-library
//
//  Created by Zinge on 3/23/17.
//
//

#include "EEDialogComponent.hpp"

#include <2d/CCNode.h>

namespace ee {
namespace dialog {
const std::string DialogComponent::DefaultName =
    "___ee_library_dialog_component_";

DialogComponent* DialogComponent::create() {
    auto result = new Self();
    result->init();
    result->autorelease();
    return result;
}

bool DialogComponent::init() {
    if (not Super::init()) {
        return false;
    }
    setName(DefaultName);
    return true;
}

DialogComponent*
DialogComponent::setPauseCallback(const PauseCallback& callback) {
    pauseCallback_ = callback;
    return this;
}

DialogComponent*
DialogComponent::setResumeCallback(const ResumeCallback& callback) {
    resumeCallback_ = callback;
    return this;
}

void DialogComponent::resume(Dialog* dialog) {
    if (resumeCallback_) {
        resumeCallback_(dialog);
    }
}

void DialogComponent::pause(Dialog* dialog) {
    if (pauseCallback_) {
        pauseCallback_(dialog);
    }
}
} // namespace dialog
} // namespace ee
