namespace ee {
namespace core {
#define Self ObserverManager

template <class Observer>
Self<Observer>::Self()
    : counter_(0) {}

template <class Observer>
Self<Observer>::~Self() {
    if (observers_.empty()) {
        // OK.
    } else {
        // Leak.
    }
}

template <class Observer>
int Self<Observer>::addObserver(const Observer& observer) {
    auto id = counter_++;
    observers_.emplace(id, observer);
    return true;
}

template <class Observer>
bool Self<Observer>::removeObserver(int id) {
    auto iter = observers_.find(id);
    if (iter == observers_.cend()) {
        return false;
    }
    observers_.erase(iter);
    return true;
}

template <class Observer>
void Self<Observer>::dispatchEvent(const Dispatcher& dispatcher) {
    for (auto&& iter : observers_) {
        dispatcher(iter.second);
    }
}

#undef Self
} // namespace core
} // namespace ee
