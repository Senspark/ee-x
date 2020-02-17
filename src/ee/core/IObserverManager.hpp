#ifndef EE_X_I_OBSERVER_MANAGER_HPP
#define EE_X_I_OBSERVER_MANAGER_HPP

#include <functional>
#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
template <class Observer>
class IObserverManager {
public:
    using Dispatcher = std::function<void(const Observer& observer)>;

    virtual ~IObserverManager() = default;

    /// Adds an observer with the specified key.
    virtual bool addObserver(const std::string& key,
                             const Observer& observer) = 0;

    /// Removes an observer with the specified key.
    virtual bool removeObserver(const std::string& key) = 0;

    /// Dispatches an event.
    virtual void dispatchEvent(const Dispatcher& dispatcher) = 0;
};
} // namespace core
} // namespace ee

#endif // EE_X_I_OBSERVER_MANAGER_HPP
