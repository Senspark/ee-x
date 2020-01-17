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

#include <ee/FirebaseFwd.hpp>

#if defined(EE_X_MOBILE)
namespace firebase {
namespace dynamic_links {
class Listener;
} // namespace dynamic_links
} // namespace firebase
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
// https://firebase.google.com/docs/dynamic-links/cpp/receive
class DynamicLink final {
private:
    using LinkReceivedCallback = std::function<void(const std::string& link)>;

public:
    DynamicLink();
    ~DynamicLink();

    explicit DynamicLink(const Logger& logger);

    bool initialize();

    /// Fetch any pending dynamic links. Each pending link will trigger a call
    /// to the registered Listener class.
    void fetch();

    void setLinkReceivedCallback(const LinkReceivedCallback& callback);

protected:
    void onLinkReceived(const std::string& link);

private:
#if defined(EE_X_MOBILE)
    class Listener;
#endif // EE_X_MOBILE

    bool initialized_;
    const Logger& logger_;
    LinkReceivedCallback callback_;
#if defined(EE_X_MOBILE)
    std::unique_ptr<::firebase::dynamic_links::Listener> listener_;
#endif // EE_X_MOBILE
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_DYNAMIC_LINK_HPP */
