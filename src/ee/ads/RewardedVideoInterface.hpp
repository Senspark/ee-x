//
//  RewardedVideoInterface.hpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#ifndef EE_X_REWARDED_VIDEO_INTERFACE_HPP
#define EE_X_REWARDED_VIDEO_INTERFACE_HPP

#include <functional>
#include <string>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
using RewardedVideoCallback = std::function<void(bool result)>;

class RewardedVideoInterface {
public:
    RewardedVideoInterface();
    virtual ~RewardedVideoInterface();

    virtual bool isLoaded() const = 0;

    virtual void load() = 0;

    virtual bool show() = 0;

    void setResultCallback(const RewardedVideoCallback& callback);

protected:
    void setResult(bool result);

private:
    RewardedVideoCallback callback_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_REWARDED_VIDEO_INTERFACE_HPP */
