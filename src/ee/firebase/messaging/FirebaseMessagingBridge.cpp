//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include "ee/firebase/messaging/FirebaseMessagingBridge.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/messaging.h>
#endif // EE_X_MOBILE

#include <ee/firebase/core/FirebaseApp.hpp>

namespace ee {
namespace firebase {
namespace messaging {
#if defined(EE_X_MOBILE)
namespace {
class Listener : public ::firebase::messaging::Listener {
private:
    using Self = Listener;
    using Message = ::firebase::messaging::Message;

public:
    using MessageCallback = std::function<void(const Message& message)>;
    using TokenCallback = std::function<void(const std::string& token)>;

    explicit Listener(const MessageCallback& messageCallback,
                      const TokenCallback& tokenCallback)
        : messageCallback_(messageCallback)
        , tokenCallback_(tokenCallback) {}

    virtual void
    OnMessage(const ::firebase::messaging::Message& message) override {
        messageCallback_(message);
    }

    virtual void OnTokenReceived(const char* token) override {
        tokenCallback_(token);
    }

private:
    MessageCallback messageCallback_;
    TokenCallback tokenCallback_;
};
} // namespace
#endif // EE_X_MOBILE

#if defined(EE_X_MOBILE)
Notification::Notification(const ::firebase::messaging::Notification& other) {
    title = other.title;
    body = other.body;
}
#endif // EE_X_MOBILE

Message::Message(const Message& other) {
    copy(other);
}

Message& Message::operator=(const Message& other) {
    if (this != std::addressof(other)) {
        copy(other);
    }
    return *this;
}

#if defined(EE_X_MOBILE)
Message::Message(const ::firebase::messaging::Message& other) {
    from = other.from;
    data = other.data;
    to = other.to;
    rawData = other.raw_data;
    messageType = other.message_type;
    error = other.error;
    errorDescription = other.error_description;
    messageId = other.message_id;
    if (other.notification != nullptr) {
        notification = std::make_unique<Notification>(*other.notification);
    }
    notificationOpened = other.notification_opened;
}
#endif // EE_X_MOBILE

void Message::copy(const Message& other) {
    from = other.from;
    data = other.data;
    to = other.to;
    rawData = other.rawData;
    messageType = other.messageType;
    error = other.error;
    errorDescription = other.errorDescription;
    messageId = other.messageId;
    notification.reset();
    if (other.notification) {
        notification = std::make_unique<Notification>(*other.notification);
    }
    notificationOpened = other.notificationOpened;
}

using Self = Bridge;

Self::Bridge() {
    initialized_ = false;
}

Self::~Bridge() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Terminate();
#endif // EE_X_MOBILE
    }
}

bool Self::initialize() {
    if (initialized_) {
        return true;
    }

    App::initialize();

#if defined(EE_X_MOBILE)
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    listener_ = std::make_unique<Listener>(
        std::bind(&Self::onMessage, this, std::placeholders::_1),
        std::bind(&Self::onTokenReceived, this, std::placeholders::_1));

    auto initResult = ::firebase::messaging::Initialize(*app, listener_.get());
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }
#endif // EE_X_MOBILE

    initialized_ = true;
    return true;
}

bool Self::initialize(const TokenCallback& callback) {
    setTokenCallback(callback);
    return initialize();
}

void Self::setMessageCallback(const MessageCallback& callback) {
    messageCallback_ = callback;
}

void Self::setTokenCallback(const TokenCallback& callback) {
    tokenCallback_ = callback;
}

#if defined(EE_X_MOBILE)
void Self::onMessage(const ::firebase::messaging::Message& message) {
    if (messageCallback_) {
        messageCallback_(Message(message));
    }
}

void Self::onTokenReceived(const std::string& token) {
    if (tokenCallback_) {
        tokenCallback_(token);
    }
}
#endif // EE_X_MOBILE

void Self::subscribe(const std::string& topic) {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Subscribe(topic.c_str());
#endif // EE_X_MOBILE
    }
}

void Self::unsubscribe(const std::string& topic) {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Unsubscribe(topic.c_str());
#endif // EE_X_MOBILE
    }
}
} // namespace messaging
} // namespace firebase
} // namespace ee
