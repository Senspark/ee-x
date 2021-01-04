#ifndef EE_X_ERROR_HANDLER_HPP
#define EE_X_ERROR_HANDLER_HPP

#include <functional>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
class Retrier {
public:
    using Processor = std::function<Task<bool>()>;

    explicit Retrier(float base, float multipler, float limit);

    Task<> process(const Processor& processor);
    void stop();

private:
    float base_;
    float multiplier_;
    float limit_;
    bool processing_;
    bool stopRequested_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_ERROR_HANDLER_HPP