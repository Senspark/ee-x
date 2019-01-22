//
//  jsb_rewarded_video.hpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#ifndef JSB_REWARDED_VIDEO_HPP__
#define JSB_REWARDED_VIDEO_HPP__

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace ads {
se::Class* getIRewardedVideoClass();

bool register_rewarded_video_manual(se::Object* object);
} // namespace ads
} // namespace ee

#endif /* JSB_REWARDED_VIDEO_HPP__ */
