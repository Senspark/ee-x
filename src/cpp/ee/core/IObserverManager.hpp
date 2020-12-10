#ifndef EE_X_I_OBSERVER_MANAGER_HPP
#define EE_X_I_OBSERVER_MANAGER_HPP

#ifdef __cplusplus

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

    /// Adds an observer.
    /// @returns Observer's ID.
    virtual int addObserver(const Observer& observer) = 0;

    /// Removes an observer with the specified ID.
    virtual bool removeObserver(int id) = 0;

    /// Dispatches an event.
    virtual void dispatchEvent(const Dispatcher& dispatcher) = 0;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_OBSERVER_MANAGER_HPP
