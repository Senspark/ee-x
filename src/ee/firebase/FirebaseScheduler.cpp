//
//  FirebaseScheduler.cpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseScheduler.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/future.h>

namespace ee {
namespace firebase {
template <class T> FirebaseScheduler<T>::FirebaseScheduler() {
    counter_ = 0;
    getSchedulers().insert(this);
}

template <class T> FirebaseScheduler<T>::~FirebaseScheduler() {
    getSchedulers().erase(this);
}

template <class T>
void FirebaseScheduler<T>::push(const FutureType& future,
                                const CallbackType& callback) {
    futures_.emplace(std::piecewise_construct,
                     std::forward_as_tuple(counter_++),
                     std::forward_as_tuple(future, callback));
    future.OnCompletion(onCompletion, static_cast<void*>(this));
}

template <class T>
std::set<FirebaseScheduler<T>*>& FirebaseScheduler<T>::getSchedulers() {
    static std::set<FirebaseScheduler*> schedulers;
    return schedulers;
}

template <class T>
void FirebaseScheduler<T>::onCompletion(const FutureType& future,
                                        void* userData) {
    auto scheduler = static_cast<FirebaseScheduler*>(userData);
    if (getSchedulers().count(scheduler) == 0) {
        return;
    }
    auto&& futures = scheduler->futures_;
    for (auto it = futures.begin(); it != futures.end();) {
        auto&& fut = it->second.first;
        auto status = fut.status();
        if (status == ::firebase::FutureStatus::kFutureStatusPending) {
            ++it;
            continue;
        }
        if (status == ::firebase::FutureStatus::kFutureStatusComplete) {
            auto&& callback = it->second.second;
            callback(fut);
            it = futures.erase(it);
        }
        if (status == ::firebase::FutureStatus::kFutureStatusInvalid) {
            it = futures.erase(it);
        }
    }
}

template class FirebaseScheduler<void>;
} // namespace firebase
} // namespace ee

#endif // EE_X_MOBILE
