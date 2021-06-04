//
//  NullRetrier.hpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#ifndef EE_X_NULL_RETRIER_HPP
#define EE_X_NULL_RETRIER_HPP

#include "ee/ads/internal/IRetrier.hpp"

namespace ee {
namespace ads {
class NullRetrier : public IRetrier {
public:
    virtual Task<> process(const Processor& processor) override;
    virtual void stop() override;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_NULL_RETRIER_HPP */
