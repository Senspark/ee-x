//
//  Created by eps on 12/6/19.
//

#ifndef EE_X_TENJIN_BRIDGE_HPP
#define EE_X_TENJIN_BRIDGE_HPP

#include "ee/CoreFwd.hpp"
#include "ee/tenjin/ITenjinBridge.hpp"

namespace ee {
namespace tenjin {
class Bridge final : public IBridge {
public:
    Bridge();
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void initialize(const std::string& apiKey) override;

private:
    IMessageBridge& bridge_;
};
} // namespace tenjin
} // namespace ee

#endif /* EE_X_TENJIN_BRIDGE_HPP */
