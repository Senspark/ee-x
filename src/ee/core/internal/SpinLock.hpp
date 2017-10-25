//
//  SpinLock.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_CORE_SPIN_LOCK_HPP_
#define EE_X_CORE_SPIN_LOCK_HPP_

#include <atomic>

namespace ee {
namespace core {
/// http://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-atomic
/// http://stackoverflow.com/questions/30691135/basic-spin-lock-mutex-implementation-ordering
class SpinLock final {
public:
    SpinLock() = default;
    ~SpinLock() = default;

    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    void lock() {
        while (locked_.test_and_set(std::memory_order::memory_order_acquire)) {
        }
    }

    void unlock() { locked_.clear(std::memory_order::memory_order_release); }

private:
    std::atomic_flag locked_ = ATOMIC_FLAG_INIT;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_SPIN_LOCK_HPP_ */
