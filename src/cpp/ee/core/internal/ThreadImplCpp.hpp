#ifndef EE_X_THREAD_IMPL_CPP_HPP
#define EE_X_THREAD_IMPL_CPP_HPP

#include <thread>

#include "ee/core/internal/IThreadImpl.hpp"

namespace ee {
namespace core {
class ThreadImplCpp : public IThreadImpl {
public:
    virtual void initialize() override;
    virtual bool isLibraryThread() const override;
    virtual bool
    runOnLibraryThread(const std::function<void()>& runnable) override;

private:
    std::thread::id id_;
};
} // namespace core
} // namespace ee

#endif // EE_X_THREAD_IMPL_CPP_HPP
