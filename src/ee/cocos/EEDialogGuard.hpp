//
//  DialogGuard.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef EE_LIBRARY_DIALOG_GUARD_HPP_
#define EE_LIBRARY_DIALOG_GUARD_HPP_

#include <2d/CCNode.h>
#include <base/CCRefPtr.h>

namespace ee {
namespace dialog {
class Dialog;

/// Guards the dialog by keep a reference to its container.
struct Guard final {
    /// Constructs a guard for the specified dialog.
    explicit Guard(Dialog* dialog);

    Dialog* dialog;

private:
    cocos2d::RefPtr<cocos2d::Node> guard_;
};
} // namespace dialog
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_GUARD_HPP_ */
