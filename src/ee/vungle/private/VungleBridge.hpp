#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#include <map>

#include "ee/vungle/IVungleBridge.hpp"

namespace ee {
namespace vungle {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;
    virtual Task<bool> initialize(const std::string& appId) override;
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend RewardedAd;

    bool destroyRewardedAd(const std::string& adId);

    bool hasRewardedAd(const std::string& adId) const;
    void loadRewardedAd(const std::string& adId) const;
    void showRewardedAd(const std::string& adId);

    void onLoaded(const std::string& adId);
    void onFailedToLoad(const std::string& adId, const std::string& message);
    void onFailedToShow(const std::string& adId, const std::string& message);
    void onClicked(const std::string& adId);
    void onClosed(const std::string& adId, bool rewarded);

    void onMediationAdClosed(const std::string& adId, bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    template <class Strong, class Weak>
    struct Ad {
        std::shared_ptr<Strong> strong;
        Weak* weak;

        explicit Ad(const std::shared_ptr<Strong>& strong_, Weak* weak_)
            : strong(strong_)
            , weak(weak_) {}
    };

    std::map<std::string, Ad<IRewardedAd, RewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
