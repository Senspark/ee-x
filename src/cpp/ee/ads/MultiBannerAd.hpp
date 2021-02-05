//
//  MultiBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/18/17.
//
//

#ifndef EE_X_MULTI_AD_VIEW_HPP
#define EE_X_MULTI_AD_VIEW_HPP

#ifdef __cplusplus

#include <memory>
#include <set>
#include <utility>
#include <vector>

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IBannerAd.hpp"

namespace ee {
namespace ads {
class MultiBannerAd : public IBannerAd, public ObserverManager<AdObserver> {
private:
    using Self = MultiBannerAd;

public:
    MultiBannerAd();
    virtual ~MultiBannerAd() override;

    Self& addItem(const std::shared_ptr<IBannerAd>& item);

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<float, float> getPosition() const override;
    virtual void setPosition(float x, float y) override;

    virtual std::pair<float, float> getSize() const override;
    virtual void setSize(float width, float height) override;

private:
    /// Whether visible.
    bool visible_;

    /// Custom anchor.
    std::pair<float, float> anchor_;

    /// Custom position.
    std::pair<float, float> position_;

    std::vector<std::shared_ptr<IBannerAd>> items_;
    std::unique_ptr<ObserverHandle> handle_;

    std::shared_ptr<IBannerAd> activeItem_;
    std::set<std::shared_ptr<IBannerAd>> loadedItems_;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_MULTI_AD_VIEW_HPP */
