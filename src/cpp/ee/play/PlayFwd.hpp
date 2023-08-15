//
//  PlayFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_PLAY_FWD_HPP
#define EE_X_PLAY_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>
#include "ee/play/ICloudDataHandler.h"

namespace ee {
namespace play {
class IBridge;
class Bridge;
} // namespace play

using IPlay = play::IBridge;
using ICloudDataHandler = play::ICloudDataHandler;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLAY_FWD_HPP */
