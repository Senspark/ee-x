//
//  IRetrier.hpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#ifndef EE_X_I_RETRIER_HPP
#define EE_X_I_RETRIER_HPP

#include <functional>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
class IRetrier {
public:
    using Processor = std::function<Task<bool>()>;

    virtual Task<> process(const Processor& processor) = 0;
    virtual void stop() = 0;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_RETRIER_HPP */
