//
//  EEDialogManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#ifndef EE_X_LEGACY_DIALOG_MANAGER_HPP
#define EE_X_LEGACY_DIALOG_MANAGER_HPP

#include "ee/CocosFwd.hpp"

namespace ee {
namespace cocos {
class LegacyDialogManager {
public:
    static const std::shared_ptr<IDialogManager>& getInstance();

    static const std::shared_ptr<IDialogManager>&
    getManager(cocos2d::Scene* scene);
};
} // namespace cocos

using DialogManager = cocos::LegacyDialogManager;
} // namespace ee

#endif /* EE_X_LEGACY_DIALOG_MANAGER_HPP */
