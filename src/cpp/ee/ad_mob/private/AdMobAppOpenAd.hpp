#ifndef EE_X_ADMOB_APP_OPEN_AD_HPP
#define EE_X_ADMOB_APP_OPEN_AD_HPP

#include <string>

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/ads/internal/MessageHelper.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/ad_mob/AdMobFwd.hpp"

namespace ee {
namespace admob {
class AppOpenAd final : public IFullScreenAd,
                        public ObserverManager<AdObserver> {
public:
    explicit AppOpenAd(
        IMessageBridge& bridge, const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>>& displayer,
        Bridge* plugin, const std::string& adId);
    virtual ~AppOpenAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;

private:
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClosed();

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace admob
} // namespace ee

#endif // EE_X_ADMOB_APP_OPEN_AD_HPP
