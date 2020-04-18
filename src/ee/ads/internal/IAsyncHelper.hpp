#ifndef EE_X_I_ASYNC_HELPER_HPP
#define EE_X_I_ASYNC_HELPER_HPP

#include <functional>
#include <memory>

#include <ee/CoroutineFwd.hpp>

namespace ee {
namespace ads {
template <class Result>
class IAsyncHelper {
public:
    using Processor = std::function<void()>;
    using Finalizer = std::function<void(Result result)>;

    virtual ~IAsyncHelper() = default;

    /// Checks whether this helper is processing.
    virtual bool isProcessing() const = 0;

    /// Asynchronously processes.
    virtual Task<Result> process(const Processor& processor,
                                 const Finalizer& finalizer) = 0;

    /// Resolves the result.
    virtual void resolve(Result result) = 0;
};
} // namespace ads
} // namespace ee

#endif // EE_X_I_ASYNC_HELPER_HPP