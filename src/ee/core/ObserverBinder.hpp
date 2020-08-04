#ifndef EE_X_OBSERVER_BINDER_HPP
#define EE_X_OBSERVER_BINDER_HPP

#ifdef __cplusplus

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
template <class Observer>
class ObserverBinder {
public:
    explicit ObserverBinder(ObserverHandle& handle,
                            IObserverManager<Observer>& manager);

    ObserverBinder& addObserver(const Observer& observer);

private:
    ObserverHandle& handle_;
    IObserverManager<Observer>& manager_;
};
} // namespace core
} // namespace ee

#include "ee/core/ObserverBinder.inl"

#endif // __cplusplus

#endif // EE_X_OBSERVER_BINDER_HPP
