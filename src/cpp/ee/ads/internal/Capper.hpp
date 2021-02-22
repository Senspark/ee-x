//
//  Capper.hpp
//  Pods
//
//  Created by eps on 2/22/21.
//

#ifndef EE_X_CAPPER_HPP
#define EE_X_CAPPER_HPP

#include "ee/ads/internal/ICapper.hpp"

namespace ee {
namespace ads {
class Capper : public ICapper {
public:
    explicit Capper(float interval);
    virtual ~Capper() override;

    virtual bool isCapped() const override;
    virtual void cap() override;

private:
    float interval_;
    bool capped_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_CAPPER_HPP */
