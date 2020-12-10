//
//  DialogGuard.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef EE_X_DIALOG_GUARD_HPP
#define EE_X_DIALOG_GUARD_HPP

#ifdef __cplusplus

#include <base/CCRefPtr.h>

#include "ee/CocosFwd.hpp"

namespace ee {
namespace cocos {
/// Guards the dialog by keep a reference to its container.
struct DialogGuard final {
public:
    /// Constructs a guard for the specified dialog.
    explicit DialogGuard(Dialog* dialog);

    Dialog* dialog;

private:
    cocos2d::RefPtr<cocos2d::Ref> guard_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_DIALOG_GUARD_HPP */
