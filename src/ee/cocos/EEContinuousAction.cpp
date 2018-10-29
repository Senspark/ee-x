//
//  EEContinuousAction.cpp
//  ee-library
//
//  Created by Zinge on 5/12/17.
//
//

#include "EEContinuousAction.hpp"

namespace ee {
ContinuousAction& ContinuousAction::addAction(const TailAction& tailAction) {
    actions_.push_back(tailAction);
    return *this;
}

ContinuousAction::Action ContinuousAction::build() const {
    Action result = [] {
        // Empty.
    };
    for (std::size_t i = actions_.size() - 1; ~i; --i) {
        auto&& action = actions_[i];
        result = [action, result] { action(result); };
    }
    return result;
}
} // namespace ee
