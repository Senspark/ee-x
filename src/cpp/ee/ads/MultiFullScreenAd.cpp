#include "ee/ads/MultiFullScreenAd.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = MultiFullScreenAd;

Self::MultiFullScreenAd() {
    handle_ = std::make_unique<ObserverHandle>();
}

Self::~MultiFullScreenAd() = default;

Self& Self::addItem(const std::shared_ptr<IFullScreenAd>& item) {
    items_.push_back(item);
    (*handle_).bind(*item).addObserver({
        .onLoaded =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onLoaded) {
                        observer.onLoaded();
                    }
                });
            },
        .onClicked =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onClicked) {
                        observer.onClicked();
                    }
                });
            },
    });
    return *this;
}

void Self::destroy() {
    for (auto&& item : items_) {
        item->destroy();
    }
    items_.clear();
    handle_->clear();
}

bool Self::isLoaded() const {
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            return true;
        }
    }
    return false;
}

Task<bool> Self::load() {
    bool result = false;
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            continue;
        }
        if (co_await item->load()) {
            result = true;
        }
    }
    co_return result;
}

Task<FullScreenAdResult> Self::show() {
    auto result = FullScreenAdResult::Failed;
    for (auto&& item : items_) {
        if (result == FullScreenAdResult::Failed) {
            // Only process if there isn't any successfully displayed ad.
            result = co_await item->show();
        }
        if (not item->isLoaded()) {
            // Load in background.
            noAwait(item->load());
        }
    }
    co_return result;
}
} // namespace ads
} // namespace ee
