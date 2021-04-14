//
//  ICapper.hpp
//  Pods
//
//  Created by eps on 2/22/21.
//

#ifndef EE_X_I_CAPPER_HPP
#define EE_X_I_CAPPER_HPP

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
class ICapper {
public:
    virtual ~ICapper() = default;
    virtual bool isCapped() const = 0;
    virtual void cap() = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_I_CAPPER_HPP */
