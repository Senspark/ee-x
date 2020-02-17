namespace ee {
namespace core {
#define Self ObserverManager

template <class Observer>
bool Self<Observer>::addObserver(const std::string& key,
                                 const Observer& observer) {
    if (observers_.count(key) != 0) {
        return false;
    }
    observers_.emplace(key, observer);
    return true;
}

template <class Observer>
bool Self<Observer>::removeObserver(const std::string& key) {
    auto iter = observers_.find(key);
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
