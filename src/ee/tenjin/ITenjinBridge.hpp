//
//  Created by eps on 12/6/19.
//

#ifndef EE_X_I_TENJIN_BRIDGE_HPP
#define EE_X_I_TENJIN_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include "ee/TenjinFwd.hpp"

namespace ee {
namespace tenjin {
class IBridge {
public:
    virtual ~IBridge() = default;

    virtual void initialize(const std::string& apiKey) = 0;
};
} // namespace tenjin
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_TENJIN_BRIDGE_HPP */
