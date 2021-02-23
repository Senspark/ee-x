//
//  DefaultFullScreenAd.hpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#ifndef EE_X_DEFAULT_FULL_SCREEN_AD_HPP
#define EE_X_DEFAULT_FULL_SCREEN_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IFullScreenAd.hpp"
#include "ee/ads/internal/MessageHelper.hpp"

namespace ee {
namespace ads {
class DefaultFullScreenAd : public IFullScreenAd,
                            public ObserverManager<AdObserver> {
public:
    using Destroyer = std::function<void()>;
    using ResultParser = std::function<AdResult(const std::string& message)>;

    explicit DefaultFullScreenAd(
        const std::string& prefix, IMessageBridge& bridge, ILogger& logger,
        const std::shared_ptr<IAsyncHelper<AdResult>>& displayer,
        const Destroyer& destroyer, const ResultParser& resultParser,
        const std::string& adId);

    virtual ~DefaultFullScreenAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<AdResult> show() override;

private:
    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed(AdResult result);

    std::string prefix_;
    IMessageBridge& bridge_;
    ILogger& logger_;
    std::shared_ptr<IAsyncHelper<AdResult>> displayer_;
    Destroyer destroyer_;
    ResultParser resultParser_;
    std::string adId_;
    MessageHelper messageHelper_;
    std::shared_ptr<ICapper> loadCapper_;
    std::unique_ptr<IAsyncHelper<bool>> loader_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_DEFAULT_FULL_SCREEN_AD_HPP */
