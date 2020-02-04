//
//  jsb_rewarded_video.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef EE_X_JSB_I_REWARDED_VIDEO_HPP
#define EE_X_JSB_I_REWARDED_VIDEO_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace ads {
se::Class* getIRewardedVideoClass();

bool registerJsbIRewardedVideo(se::Object* global);
} // namespace ads
} // namespace ee

#endif /* EE_X_JSB_I_REWARDED_VIDEO_HPP */
