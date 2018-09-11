//
//  FirebaseMessenging.hpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#ifndef EE_X_FIREBASE_MESSAGING_HPP
#define EE_X_FIREBASE_MESSAGING_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "ee/Macro.hpp"

#if defined(EE_X_MOBILE)
namespace firebase {
namespace messaging {
struct Message;
struct Notification;
class Listener;
} // namespace messaging
} // namespace firebase
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
/// Similar to ::firebase:messaging::Notification.
/// Used for messages that display a notification.
/// On android, this requires that the app is using the Play Services client
/// library.
class Notification {
public:
    Notification() = default;
    Notification(const Notification&) = default;
    Notification& operator=(const Notification&) = default;

#if defined(EE_X_MOBILE)
    explicit Notification(const ::firebase::messaging::Notification& other);
#endif // EE_X_MOBILE

    /// Indicates notification title. This field is not visible on iOS phones
    /// and tablets.
    std::string title;

    /// Indicates notification body text.
    std::string body;
};

/// Similar to ::firebase::messaging::Message.
/// @brief Data structure used to send messages to, and receive messages from,
/// cloud messaging.
class Message {
public:
    Message() = default;
    Message(const Message& other);
    Message& operator=(const Message& other);

#if defined(EE_X_MOBILE)
    Message(const ::firebase::messaging::Message& other);
#endif // EE_X_MOBILE

    /// Authenticated ID of the sender. This is a project number in most cases.
    std::string from;
    
    /// This parameter specifies the recipient of a message.
    std::string to;
    
    /// Binary payload. This field is currently unused.
    std::string rawData;
    
    /// Equivalent with a content-type.
    /// This field is only used for downstream messages received through
    std::string messageType;
    
    /// Error code.
    std::string error;
    
    /// Human readable details about the error.
    std::string errorDescription;


    /// The metadata, including all original key/value pairs. Includes some of
    /// the HTTP headers used when sending the message. `gcm`, `google` and
    /// `goog` prefixes are reserved for internal use.
    std::map<std::string, std::string> data;

    /// Message ID. This can be specified by sender. Internally a hash of the
    /// message ID and other elements will be used for storage. The ID must be
    /// unique for each topic subscription - using the same ID may result in
    /// overriding the original message or duplicate delivery.
    std::string messageId;

    /// Optional notification to show. This only set if a notification was
    /// received with this message, otherwise it is null.
    std::unique_ptr<Notification> notification;

    /// A flag indicating whether this message was opened by tapping a
    /// notification in the OS system tray. If the message was received this way
    /// this flag is set to true.
    bool notificationOpened;

private:
    void copy(const Message& other);
};

/// https://firebase.google.com/docs/cloud-messaging/cpp/client
/// https://firebase.google.com/docs/reference/cpp/namespace/firebase/messaging
class Messaging final {
private:
    using Self = Messaging;

public:
    using MessageCallback = std::function<void(const Message& message)>;
    using TokenCallback = std::function<void(const std::string& token)>;

    Messaging();
    ~Messaging();

    bool initialize();

    bool initialize(const TokenCallback& callback);

    /// Called on the client when a message arrives.
    void setMessageCallback(const MessageCallback& callback);

    /// Called on the client when a registration token arrives. This function
    /// will eventually be called in response to a call to initialize.
    void setTokenCallback(const TokenCallback& callback);

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

    MessageCallback messageCallback_;
    TokenCallback tokenCallback_;

#if defined(EE_X_MOBILE)
    void onMessage(const ::firebase::messaging::Message& message);
    void onTokenReceived(const std::string& token);

    std::unique_ptr<::firebase::messaging::Listener> listener_;
#endif // EE_X_MOBILE
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_MESSAGING_HPP */
