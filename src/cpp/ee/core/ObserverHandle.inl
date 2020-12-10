#ifdef __cplusplus

#include "ee/core/ObserverBinder.hpp"

namespace ee {
namespace core {
#define Self ObserverHandle

template <class Observer>
ObserverBinder<Observer> Self::bind(IObserverManager<Observer>& manager) {
    return ObserverBinder<Observer>(*this, manager);
}

#undef Self
} // namespace core
} // namespace ee

#endif // __cplusplus
