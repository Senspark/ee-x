//
//  LockCapper.hpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#ifndef EE_X_LOCK_CAPPER_HPP
#define EE_X_LOCK_CAPPER_HPP

#include "ee/ads/internal/ICapper.hpp"

namespace ee {
namespace ads {
class LockCapper : public ICapper {
public:
    LockCapper();
    virtual ~LockCapper() override;

    virtual bool isCapped() const override;
    virtual void cap() override;
    virtual void lock() override;
    virtual void unlock() override;

private:
    bool locked_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_LOCK_CAPPER_HPP */
