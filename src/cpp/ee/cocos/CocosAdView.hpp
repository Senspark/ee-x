//
//  CocosAdView.hpp
//  Pods
//
//  Created by eps on 6/18/20.
//

#ifndef EE_X_COCOS_AD_VIEW_HPP
#define EE_X_COCOS_AD_VIEW_HPP

#ifdef __cplusplus

#include <ee/ads/IAdView.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
class CocosAdView : public IAdView, public ObserverManager<AdObserver> {
public:
    explicit CocosAdView(const std::shared_ptr<IAdView>& ad);

    virtual ~CocosAdView() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;

    virtual std::pair<float, float> getAnchor() const override;
    virtual void setAnchor(float x, float y) override;

    virtual std::pair<float, float> getPosition() const override;
    virtual void setPosition(float x, float y) override;

    virtual std::pair<float, float> getSize() const override;
    virtual void setSize(float width, float height) override;

    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    std::shared_ptr<IAdView> ad_;
    std::unique_ptr<ObserverHandle> handle_;
    std::unique_ptr<Metrics> metrics_;
    float sceneHeight_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_COCOS_AD_VIEW_HPP */
