//
//  FirebaseScheduler.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_SCHEDULER_HPP_
#define EE_X_FIREBASE_SCHEDULER_HPP_

#include <cstddef>
#include <functional>
#include <map>
#include <set>

#include "ee/FirebaseFwd.hpp"

namespace firebase {
template <class ResultType>
class Future;
} // namespace firebase

namespace ee {
namespace firebase {
template <class ResultType>
class Scheduler {
public:
    using FutureType = ::firebase::Future<ResultType>;
    using CallbackType = std::function<void(const FutureType& future)>;

    Scheduler();
    ~Scheduler();

    void push(const FutureType& future, const CallbackType& callback);

private:
    static std::set<Scheduler*>& getSchedulers();
    static void onCompletion(const FutureType& future, void* userData);

    std::size_t counter_;
    std::map<std::size_t, std::pair<FutureType, CallbackType>> futures_;
};
} // namespace firebase
} // namespace ee

#include "ee/firebase/core/FirebaseScheduler_inl.hpp"

#endif /* EE_X_FIREBASE_SCHEDULER_HPP_ */
