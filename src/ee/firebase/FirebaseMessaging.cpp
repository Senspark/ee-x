//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include "ee/firebase/FirebaseMessaging.hpp"
#include "ee/firebase/FirebaseApp.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/messaging.h>
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
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

FirebaseMessaging::FirebaseMessaging() {
    initialized_ = false;
}

FirebaseMessaging::~FirebaseMessaging() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Terminate();
#endif // EE_X_MOBILE
    }
}
bool FirebaseMessaging::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();

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

#if defined(EE_X_MOBILE)
void FirebaseMessaging::onMessage(
    const ::firebase::messaging::Message& message) {
    int x = 1;
}

void FirebaseMessaging::onTokenReceived(const std::string& token) {
    int x = 1;
}
#endif // EE_X_MOBILE

void FirebaseMessaging::subscribe(const std::string& topic) {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Subscribe(topic.c_str());
#endif // EE_X_MOBILE
    }
}

void FirebaseMessaging::unsubscribe(const std::string& topic) {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::messaging::Unsubscribe(topic.c_str());
#endif // EE_X_MOBILE
    }
}
} // namespace firebase
} // namespace ee
