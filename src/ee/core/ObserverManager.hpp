#ifndef EE_X_OBSERVER_MANAGER_HPP
#define EE_X_OBSERVER_MANAGER_HPP

#include <unordered_map>

#include "ee/CoreFwd.hpp"
#include "ee/core/IObserverManager.hpp"

namespace ee {
namespace core {
template <class Observer>
class ObserverManager : public virtual IObserverManager<Observer> {
public:
    using typename IObserverManager<Observer>::Dispatcher;

    ObserverManager();
    virtual ~ObserverManager() override;

    virtual int addObserver(const Observer& observer) override;

    virtual bool removeObserver(int id) override;

    virtual void dispatchEvent(const Dispatcher& dispatcher) override;

private:
    int counter_;
    std::unordered_map<int, Observer> observers_;
};
} // namespace core
} // namespace ee

#include "ee/core/ObserverManager.inl"

#endif // EE_X_OBSERVER_MANAGER_HPP
