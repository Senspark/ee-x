//
//  AdMobRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_ADMOB_REWARDED_VIDEO_HPP
#define EE_X_ADMOB_REWARDED_VIDEO_HPP

#include <ee/CoreFwd.hpp>
#include <ee/ads/IRewardedVideo.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(IMessageBridge& bridge, const Logger& logger, AdMob* plugin,
                           const std::string& adId);
    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow();
    void onReward();
    void onClosed();
private:
    friend AdMob;
    
    /// Whether the ad is in loading progress.
    bool loading_;
    bool rewarded_;
    
    bool errored_;

    IMessageBridge& bridge_;
    const Logger& logger_;
    AdMob* plugin_;
    std::string adId_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_REWARDED_VIDEO_HPP */
