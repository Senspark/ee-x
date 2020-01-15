//
//  IRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#ifndef EE_X_I_REWARDED_VIDEO_HPP
#define EE_X_I_REWARDED_VIDEO_HPP

#include <functional>
#include <string>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
using RewardedVideoCallback = std::function<void(bool result)>;
using OnRewardClickedCallback = std::function<void()>;

class IRewardedVideo {
public:
    explicit IRewardedVideo(const Logger& logger);

    virtual ~IRewardedVideo();

    virtual bool isLoaded() const = 0;

    virtual void load() = 0;

    virtual bool show() = 0;

    void setResultCallback(const RewardedVideoCallback& callback);
    virtual void setOnClickedCallback(const OnRewardClickedCallback& callback);

    void doOnClicked();

protected:
    friend MediationManager;

    void setResult(bool result);

private:
    const Logger& logger_;
    RewardedVideoCallback callback_;
    OnRewardClickedCallback onClickedCallback_{nullptr};
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_REWARDED_VIDEO_HPP */
