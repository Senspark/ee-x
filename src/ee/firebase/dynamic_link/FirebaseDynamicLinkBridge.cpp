//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include "ee/firebase/dynamic_link/FirebaseDynamicLinkBridge.hpp"

#include <firebase/dynamic_links.h>

#include <ee/core/Logger.hpp>
#include <ee/firebase/core/FirebaseCoreBridge.hpp>

namespace ee {
namespace firebase {
namespace dynamic_link {
using Self = Bridge;

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

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : logger_(logger) {
    initialized_ = false;
}

Self::~Bridge() {
    if (initialized_) {
        ::firebase::dynamic_links::Terminate();
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

    auto initResult = ::firebase::dynamic_links::Initialize(*app, nullptr);
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }

    initialized_ = true;
    return true;
}

void Self::fetch() {
    ::firebase::dynamic_links::Fetch();
}

void Self::setLinkReceivedCallback(const LinkReceivedCallback& callback) {
    callback_ = callback;
    ::firebase::dynamic_links::SetListener(nullptr);
    auto listener = std::make_unique<Listener>(
        std::bind(&Self::onLinkReceived, this, std::placeholders::_1));
    ::firebase::dynamic_links::SetListener(listener.get());
    listener_ = std::move(listener);
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
