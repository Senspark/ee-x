//
//  EEDialogUtils.hpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#ifndef EE_LIBRARY_DIALOG_UTILS_HPP_
#define EE_LIBRARY_DIALOG_UTILS_HPP_

#include "EEMacro.hpp"

NS_EE_BEGIN
namespace dialog {
class Dialog;
} // namespace dialog

using dialog::Dialog;

/// Adds a transition action for the specified dialog that:
///
/// - Blurs the background when the dialog is about to show with the specified
/// @c scale ratio and @c blurRadius and fade in the background in @c duration
/// seconds.
///
/// - Fades out the blurred background when the dialog is about to hide in
/// @c duration seconds.
///
/// @param duration     The transition duration.
/// @param scale        For blur algorithm.
/// @param blurRadius   For blur algorithm.
/// @see @c ee::captureBlurredScreenInPoints.
Dialog* addBlurBackground(Dialog* dialog, float duration = 0.8f,
                          float scale = 0.25f, int blurRadius = 2);

/// Adds a transition action for the specified dialog that:
///
/// - Moves the dialog from left to right when the dialog is about to show/hide
/// in @c duration seconds.
///
/// @param duration     The transition duration.
/// @param rate         Used in @c cocos2d::EaseElasticOut and @c
///                     cocos2d::EaseElasticIn.
Dialog* addHorizontalTransition(Dialog* dialog, float duration = 0.8f,
                                float rate = 0.8f);
NS_EE_END

#endif /* EE_LIBRARY_DIALOG_UTILS_HPP_ */
