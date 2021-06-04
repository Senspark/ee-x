#ifndef EE_X_RETRIER_HPP
#define EE_X_RETRIER_HPP

#include "ee/ads/internal/IRetrier.hpp"

namespace ee {
namespace ads {
class Retrier : public IRetrier {
public:
    explicit Retrier(float base, float multipler, float limit);

    virtual Task<> process(const Processor& processor) override;
    virtual void stop() override;

private:
    float base_;
    float multiplier_;
    float limit_;
    bool processing_;
    bool stopRequested_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_RETRIER_HPP
