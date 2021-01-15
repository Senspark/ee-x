#ifndef EE_X_I_THREAD_IMPL_HPP
#define EE_X_I_THREAD_IMPL_HPP

#include <functional>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class IThreadImpl {
public:
    virtual ~IThreadImpl() = default;
    virtual void initialize() = 0;
    virtual bool isLibraryThread() const = 0;
    virtual bool runOnLibraryThread(const std::function<void()>& runnable) = 0;
};
} // namespace core
} // namespace ee

#endif // EE_X_I_THREAD_IMPL_HPP