//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include "ee/firebase/dynamic_link/FirebaseDynamicLinkBridge.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/dynamic_links.h>
#endif // EE_X_MOBILE

#include <ee/core/Logger.hpp>
#include <ee/firebase/core/FirebaseApp.hpp>

namespace ee {
namespace firebase {
namespace dynamic_link {
using Self = Bridge;

#if defined(EE_X_MOBILE)
class Self::Listener : public ::firebase::dynamic_links::Listener {
public:
    explicit Listener(const LinkReceivedCallback& callback)
        : callback_(callback) {}

    virtual void OnDynamicLinkReceived(
        const ::firebase::dynamic_links::DynamicLink* link) override {
        callback_(link->url);
    }

private:
    LinkReceivedCallback callback_;
};
#endif // EE_X_MOBILE

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : logger_(logger) {
    initialized_ = false;
}

Self::~Bridge() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::dynamic_links::Terminate();
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

    auto initResult = ::firebase::dynamic_links::Initialize(*app, nullptr);
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }
#endif // EE_X_MOBILE

    initialized_ = true;
    return true;
}

void Self::fetch() {
#if defined(EE_X_MOBILE)
    ::firebase::dynamic_links::Fetch();
#endif // EE_X_MOBILE
}

void Self::setLinkReceivedCallback(const LinkReceivedCallback& callback) {
    callback_ = callback;
#if defined(EE_X_MOBILE)
    ::firebase::dynamic_links::SetListener(nullptr);
    auto listener = std::make_unique<Listener>(
        std::bind(&Self::onLinkReceived, this, std::placeholders::_1));
    ::firebase::dynamic_links::SetListener(listener.get());
    listener_ = std::move(listener);
#endif // EE_X_MOBILE
}

void Self::onLinkReceived(const std::string& link) {
    logger_.debug("%s %s", __PRETTY_FUNCTION__, link.c_str());
    if (callback_) {
        callback_(link);
    }
}
} // namespace dynamic_link
} // namespace firebase
} // namespace ee
