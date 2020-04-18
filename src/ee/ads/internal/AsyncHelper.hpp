#ifndef EE_X_ASYNC_HELPER_HPP
#define EE_X_ASYNC_HELPER_HPP

#include "ee/ads/internal/IAsyncHelper.hpp"

namespace ee {
namespace ads {
template <class Result>
class AsyncHelper : public IAsyncHelper<Result> {
public:
    using typename IAsyncHelper<Result>::Processor;
    using typename IAsyncHelper<Result>::Finalizer;

    virtual bool isProcessing() const override;

    virtual Task<Result> process(const Processor& processor,
                                 const Finalizer& finalizer) override;

    virtual void resolve(Result result) override;

private:
    bool processing_;
    std::unique_ptr<LambdaAwaiter<Result>> awaiter_;
    std::function<void(Result result)> resolve_;
};
} // namespace ads
} // namespace ee

#include "ee/ads/internal/AsyncHelper.inl"

#endif // EE_X_ASYNC_HELPER_HPP
