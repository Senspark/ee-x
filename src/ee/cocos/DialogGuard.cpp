//
//  DialogGuard.cpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#include "ee/cocos/DialogGuard.hpp"

#include "ee/cocos/Dialog.hpp"

namespace ee {
namespace cocos {
DialogGuard::DialogGuard(Dialog* dialog_)
    : dialog(dialog_)
    , guard_(dialog->getContainer()) {}
} // namespace cocos
} // namespace ee
