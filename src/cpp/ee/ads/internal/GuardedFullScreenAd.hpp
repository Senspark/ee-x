#ifndef EE_X_GUARDED_FULL_SCREEN_AD_HPP
#define EE_X_GUARDED_FULL_SCREEN_AD_HPP

#include "ee/ads/IFullScreenAd.hpp"
#include "ee/ads/internal/GuardedAd.hpp"

namespace ee {
namespace ads {
class GuardedFullScreenAd : public IFullScreenAd, public GuardedAd {
public:
    explicit GuardedFullScreenAd(const std::shared_ptr<IFullScreenAd>& ad,
                                 const std::shared_ptr<ICapper>& capper,
                                 const std::shared_ptr<IRetrier>& retrier);
    virtual ~GuardedFullScreenAd() override;
    virtual Task<AdResult> show() override;
    
protected:
    virtual bool isDisplaying() const override;

private:
    std::shared_ptr<IFullScreenAd> ad_;

    bool displaying_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_GUARDED_FULL_SCREEN_AD_HPP
