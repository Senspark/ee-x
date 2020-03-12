//
//  FirebaseMessenging.hpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#ifndef EE_X_FIREBASE_DYNAMIC_LINK_BRIDGE_HPP
#define EE_X_FIREBASE_DYNAMIC_LINK_BRIDGE_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <ee/FirebaseFwd.hpp>

namespace firebase {
namespace dynamic_links {
class Listener;
} // namespace dynamic_links
} // namespace firebase

namespace ee {
namespace firebase {
namespace dynamic_link {
// https://firebase.google.com/docs/dynamic-links/cpp/receive
class Bridge final {
private:
    using LinkReceivedCallback = std::function<void(const std::string& link)>;

public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    bool initialize();

    /// Fetch any pending dynamic links. Each pending link will trigger a call
    /// to the registered Listener class.
    void fetch();

    void setLinkReceivedCallback(const LinkReceivedCallback& callback);

protected:
    void onLinkReceived(const std::string& link);

private:
    class Listener;

    bool initialized_;
    const Logger& logger_;
    LinkReceivedCallback callback_;
    std::unique_ptr<::firebase::dynamic_links::Listener> listener_;
};
} // namespace dynamic_link
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_DYNAMIC_LINK_BRIDGE_HPP */
