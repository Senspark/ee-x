//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include "ee/firebase_messaging/FirebaseMessagingBridge.hpp"

#include <firebase/messaging.h>

#include <ee/firebase_core/FirebaseCoreBridge.hpp>

namespace ee {
namespace firebase {
namespace messaging {
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

Notification::Notification(const ::firebase::messaging::Notification& other) {
    title = other.title;
    body = other.body;
}

Message::Message(const Message& other) {
    copy(other);
}

Message& Message::operator=(const Message& other) {
    if (this != std::addressof(other)) {
        copy(other);
    }
    return *this;
}

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
        ::firebase::messaging::Terminate();
    }
}

bool Self::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseCore::initialize();

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

void Self::subscribe(const std::string& topic) {
    if (initialized_) {
        ::firebase::messaging::Subscribe(topic.c_str());
    }
}

void Self::unsubscribe(const std::string& topic) {
    if (initialized_) {
        ::firebase::messaging::Unsubscribe(topic.c_str());
    }
}
} // namespace messaging
} // namespace firebase
} // namespace ee
