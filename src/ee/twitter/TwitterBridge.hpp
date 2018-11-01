//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_TWITTER_BRIDGE_HPP
#define EE_X_TWITTER_BRIDGE_HPP

#include <functional>
#include <string>

#include "ee/core/LogLevel.hpp"

namespace ee {
namespace twitter {
using ShareResultCallback = std::function<void(int result)>;

class Twitter final {
public:
    Twitter();
    ~Twitter();

    void setShareResultCallback(const ShareResultCallback& callback);
    void initialize(const std::string& key, const std::string& secret);
    void shareContent(const std::string& text);
    void shareScreenShot(const std::string& text, const std::string& image);

private:
    MessageBridge& bridge_;

    void onSuccess();
    void onFailure();
    void onCancel();

    bool _hadCallFailure{false};
    ShareResultCallback _callback;
};
} // namespace twitter
} // namespace ee

#endif /* EE_X_TWITTER_BRIDGE_HPP */
