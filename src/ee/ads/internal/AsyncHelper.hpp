#ifndef EE_X_ASYNC_HELPER_HPP
#define EE_X_ASYNC_HELPER_HPP

#include <functional>
#include <memory>

#include <ee/CoroutineFwd.hpp>

namespace ee {
namespace ads {
template <class Result>
class AsyncHelper {
public:
    using Processor = std::function<void()>;

    bool isProcessing() const;
    Task<Result> process(const Processor& processor);
    void resolve(Result result);

private:
    bool processing_;
    std::unique_ptr<LambdaAwaiter<Result>> awaiter_;
    std::function<void(Result result)> resolve_;
};
} // namespace ads
} // namespace ee

#include "ee/ads/internal/AsyncHelper.inl"

#endif // EE_X_ASYNC_HELPER_HPP
