//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_FACEBOOK_BRIDGE_HPP
#define EE_X_FACEBOOK_BRIDGE_HPP

#include <functional>
#include <string>

#include "ee/facebook/IFacebookBridge.hpp"

namespace ee {
namespace facebook {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual bool isLoggedIn() const override;
    virtual Task<LoginResult>
    logIn(const std::vector<std::string>& permissions) override;
    virtual Task<> logOut() override;
    virtual std::shared_ptr<IAccessToken> getAccessToken() const override;
    virtual Task<GraphResult>
    graphRequest(const GraphRequest& request) override;
    virtual Task<ShareResult> shareContent(ShareType type,
                                           const std::string& url) override;

private:
    void registerNotifications();
    void onProfileChanged(const std::string& profile);

    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_BRIDGE_HPP */
