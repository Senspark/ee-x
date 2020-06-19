//
//  CocosRewardedAd.cpp
//  Pods
//
//  Created by eps on 6/19/20.
//

#include "ee/cocos/CocosRewardedAd.hpp"

#include <ee/core/MakeAwaiter.hpp>
#include <ee/core/ObserverHandle.hpp>

#include "ee/cocos/SwitchToCocosThread.hpp"

namespace ee {
namespace cocos {
using Self = CocosRewardedAd;

Self::CocosRewardedAd(const std::shared_ptr<IRewardedAd>& ad)
    : ad_(ad) {
    handle_ = std::make_unique<ObserverHandle>();
    handle_->bind(*ad_).addObserver({
        .onLoaded = makeAwaiter([this]() -> Task<> {
            co_await SwitchToCocosThread();

            // Propagation.
            dispatchEvent([](auto&& observer) {
                if (observer.onLoaded) {
                    observer.onLoaded();
                }
            });
        }),
        .onClicked = makeAwaiter([this]() -> Task<> {
            co_await SwitchToCocosThread();

            // Propagation.
            dispatchEvent([](auto&& observer) {
                if (observer.onClicked) {
                    observer.onClicked();
                }
            });
        }),
    });
}

Self::~CocosRewardedAd() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
}

bool Self::isLoaded() const {
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    auto result = co_await ad_->load();
    co_await SwitchToCocosThread();
    co_return result;
}

Task<IRewardedAdResult> Self::show() {
    auto result = co_await ad_->show();
    co_await SwitchToCocosThread();
    co_return result;
}
} // namespace cocos
} // namespace ee
