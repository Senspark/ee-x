#ifdef __cplusplus

#include "ee/core/ObserverHandle.hpp"

namespace ee {
namespace core {
#define Self ObserverBinder

template <class Observer>
Self<Observer>::Self(ObserverHandle& handle,
                     IObserverManager<Observer>& manager)
    : handle_(handle)
    , manager_(manager) {}

template <class Observer>
Self<Observer>& Self<Observer>::addObserver(const Observer& observer) {
    auto id = manager_.addObserver(observer);
    auto&& manager = manager_;
    handle_.add([id, &manager] { //
        manager.removeObserver(id);
    });
    return *this;
}

#undef Self
} // namespace core
} // namespace ee

#endif // __cplusplus
