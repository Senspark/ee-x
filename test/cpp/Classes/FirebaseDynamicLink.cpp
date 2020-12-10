//
//  FirebaseDynamicLink.cpp
//  ee_x_test
//
//  Created by eps on 5/16/18.
//

#include "FirebaseDynamicLink.hpp"

#include <ee/Firebase.hpp>

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
using Self = FirebaseDynamicLink;

Self::FirebaseDynamicLink() {
    plugin_ = std::make_unique<ee::FirebaseDynamicLink>();
    plugin_->initialize();
    plugin_->setLinkReceivedCallback([](const std::string& link) {
        FunctionLogger logger("Received dynamic link");
        getLogger().info("link = %s", link.c_str());
    });
    plugin_->fetch();
}
} // namespace eetest
