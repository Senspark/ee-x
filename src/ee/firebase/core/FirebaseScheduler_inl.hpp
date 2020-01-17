//
//  FirebaseScheduler.cpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_SCHEDULER_HPP_
#error "This file must be only included by FirebaseScheduler.hpp"
#endif // EE_X_FIREBASE_SCHEDULER_HPP_

#if defined(EE_X_MOBILE)
#include <firebase/future.h>

namespace ee {
namespace firebase {
template <class T>
Scheduler<T>::Scheduler() {
    counter_ = 0;
    getSchedulers().insert(this);
}

template <class T>
Scheduler<T>::~Scheduler() {
    getSchedulers().erase(this);
}

template <class T>
void Scheduler<T>::push(const FutureType& future,
                        const CallbackType& callback) {
    futures_.emplace(std::piecewise_construct,
                     std::forward_as_tuple(counter_++),
                     std::forward_as_tuple(future, callback));
    future.OnCompletion(onCompletion, static_cast<void*>(this));
}

template <class T>
std::set<Scheduler<T>*>& Scheduler<T>::getSchedulers() {
    static std::set<Scheduler*> schedulers;
    return schedulers;
}

template <class T>
void Scheduler<T>::onCompletion(const FutureType& future, void* userData) {
    auto scheduler = static_cast<Scheduler*>(userData);
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
} // namespace firebase
} // namespace ee

#endif // EE_X_MOBILE
