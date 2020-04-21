#include <mutex>

namespace ee {
namespace core {
#define Self SafeObserverManager

template <class Observer>
Self<Observer>::Self() {}

template <class Observer>
Self<Observer>::~Self() {}

template <class Observer>
int Self<Observer>::addObserver(const Observer& observer) {
    std::scoped_lock<SpinLock> lock(lock_);
    return manager_.addObserver(observer);
}

template <class Observer>
bool Self<Observer>::removeObserver(int id) {
    std::scoped_lock<SpinLock> lock(lock_);
    return manager_.removeObserver(id);
}

template <class Observer>
void Self<Observer>::dispatchEvent(const Dispatcher& dispatcher) {
    std::scoped_lock<SpinLock> lock(lock_);
    manager_.dispatchEvent(dispatcher);
}

#undef Self
} // namespace core
} // namespace ee
