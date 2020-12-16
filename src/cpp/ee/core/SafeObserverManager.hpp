#ifndef EE_X_SAFE_OBSERVER_MANAGER_HPP
#define EE_X_SAFE_OBSERVER_MANAGER_HPP

#ifdef __cplusplus

#include "ee/core/ObserverManager.hpp"
#include "ee/core/SpinLock.hpp"

namespace ee {
namespace core {
/// Thread-safe version for ObserverManager.
template <class Observer>
class SafeObserverManager : public virtual IObserverManager<Observer> {
public:
    using typename IObserverManager<Observer>::Dispatcher;

    SafeObserverManager();
    virtual ~SafeObserverManager() override;

    virtual int addObserver(const Observer& observer) override;

    virtual bool removeObserver(int id) override;

    virtual void dispatchEvent(const Dispatcher& dispatcher) override;

private:
    ObserverManager<Observer> manager_;
    SpinLock lock_;
};
} // namespace core
} // namespace ee

#include "ee/core/SafeObserverManager.inl"

#endif // __cplusplus

#endif // EE_X_SAFE_OBSERVER_MANAGER_HPP
