//
//  Created by Zinge on 7/4/16.
//
//
#include "ee/twitter/TwitterBridge.hpp"

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace twitter {
namespace {
// clang-format off
constexpr auto k__initialize        = "twitter_initialize";
constexpr auto k__shareContent      = "twitter_shareContent";
constexpr auto k__shareScreenShot   = "twitter_shareScreenShot";
constexpr auto k__onSuccess         = "twitter_onSuccess";
constexpr auto k__onFailure         = "twitter_onFailure";
constexpr auto k__onCancel          = "twitter_onCancel";
constexpr auto k__key               = "twitter_key";
constexpr auto k__secret            = "twitter_secret";
constexpr auto k__text              = "twitter_text";
constexpr auto k__image             = "twitter_image";
// clang-format on
} // namespace

using Self = Twitter;

Self::Twitter()
    : bridge_(MessageBridge::getInstance()) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            onSuccess();
            return "";
        },
        k__onSuccess);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailure();
            return "";
        },
        k__onFailure);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onCancel();
            return "";
        },
        k__onCancel);
}

Self::~Twitter() {
    //
}

void Self::initialize(const std::string& key, const std::string& secret) {
    nlohmann::json json;
    json[k__key] = key;
    json[k__secret] = secret;
    bridge_.call(k__initialize, json.dump());
}

void Self::setShareResultCallback(const ShareResultCallback& callback) {
    _callback = callback;
}

void Self::shareContent(const std::string& text) {
    nlohmann::json json;
    json[k__text] = text;
    bridge_.call(k__shareContent, json.dump());
}

void Self::shareScreenShot(const std::string& text, const std::string& image) {
    nlohmann::json json;
    json[k__text] = text;
    json[k__image] = image;
    bridge_.call(k__shareScreenShot, json.dump());
}

void Self::onSuccess() {
    if (_callback) {
        _hadCallFailure = false;
        _callback(true);
    }
}

void Self::onFailure() {
    if (_callback) {
        _hadCallFailure = true;
        _callback(false);
    }
}

void Self::onCancel() {
    if (_callback) {
        if (!_hadCallFailure) {
            _callback(false);
        }
    }
}

} // namespace twitter
} // namespace ee
