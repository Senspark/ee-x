//
//  FirebaseMessenging.hpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#ifndef EE_X_FIREBASE_MESSAGING_HPP
#define EE_X_FIREBASE_MESSAGING_HPP

#include <memory>
#include <string>

#include "ee/Macro.hpp"

#if defined(EE_X_MOBILE)
namespace firebase {
namespace messaging {
class Listener;
class Message;
} // namespace messaging
} // namespace firebase
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
class FirebaseMessaging final {
private:
    using Self = FirebaseMessaging;

public:
    FirebaseMessaging();
    ~FirebaseMessaging();

    bool initialize();

    /// Subscribe to receive all messages to the specified topic.
    /// Subscribes an app instance to a topic, enabling it to receive messages
    /// sent to that topic.
    /// Call this function from the main thread. FCM is not thread safe.
    /// @param[in] topic The topic to subscribe to. Should be of the form
    /// `"/topics/<topic-name>"`.
    void subscribe(const std::string& topic);

    /// Unsubscribe from a topic.
    /// Unsubscribes an app instance from a topic, stopping it from receiving
    /// any further messages sent to that topic.
    /// Call this function from the main thread. FCM is not thread safe.
    /// @param[in] topic The topic to unsubscribe from.
    void unsubscribe(const std::string& topic);

private:
    bool initialized_;

#if defined(EE_X_MOBILE)
    void onMessage(const ::firebase::messaging::Message& message);
    void onTokenReceived(const std::string& token);

    std::unique_ptr<::firebase::messaging::Listener> listener_;
#endif // EE_X_MOBILE
};
} // namespace firebase
} // namespac eee

#endif /* EE_X_FIREBASE_MESSAGING_HPP */
