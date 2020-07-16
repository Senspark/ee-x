//
//  SwitchToLibraryThread.hpp
//  Pods
//
//  Created by eps on 7/15/20.
//

#ifndef EE_X_SWITCH_TO_LIBRARY_THREAD_HPP
#define EE_X_SWITCH_TO_LIBRARY_THREAD_HPP

#ifdef __cplusplus

#include <experimental/coroutine>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
class SwitchToLibraryThread {
private:
    using Self = SwitchToLibraryThread;

public:
    SwitchToLibraryThread();
    ~SwitchToLibraryThread();

    SwitchToLibraryThread(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(std::experimental::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_SWITCH_TO_LIBRARY_THREAD_HPP */
