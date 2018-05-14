//
//  FirebaseMessenging.hpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#ifndef EE_X_FIREBASE_DYNAMIC_LINK_HPP
#define EE_X_FIREBASE_DYNAMIC_LINK_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <ee/core/IMessageBridge.hpp>

#include "ee/Macro.hpp"

namespace ee {
namespace firebase {

class DynamicLink final {
    using OnReceiveLinkCallback = std::function<void(std::string link)>;
    
public:
    DynamicLink();
    ~DynamicLink();

    void setupReceiver(const OnReceiveLinkCallback& callback);

    void registerHandlers();

    void deRegisterHandlers();

    void onReceiveDynamicLink(std::string link);

private:
    IMessageBridge& bridge_;
    OnReceiveLinkCallback callback_;
};

} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_DYNAMIC_LINK_HPP */
