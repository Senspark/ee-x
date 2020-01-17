//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_
#define EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_

#include <functional>
#include <string>

#include "ee/facebook/IFacebookBridge.hpp"

namespace ee {
namespace facebook {
class Bridge final : public IBridge {
public:
    Bridge();
    virtual ~Bridge() override;

    /// @see Super.
    virtual bool isLoggedIn() const override;

    /// @see Super.
    virtual void
    logIn(const std::vector<std::string>& permissions,
          const std::shared_ptr<ILoginDelegate>& delegate) override;

    /// @see Super.
    virtual std::shared_ptr<ILoginDelegate> createLoginDelegate() override;

    /// @see Super.
    virtual void logOut() override;

    /// @see Super.
    virtual std::shared_ptr<IAccessToken> getAccessToken() const override;

    /// @see Super.
    virtual void
    graphRequest(const GraphRequest& request,
                 const std::shared_ptr<IGraphDelegate>& delegate) override;

    /// @see Super.
    virtual std::shared_ptr<IGraphDelegate> createGraphDelegate() override;

    /// @see Super.
    virtual void
    sendRequest(const RequestContent& content,
                const std::shared_ptr<IRequestDelegate>& delegate) override;

    /// @see Super.
    virtual std::shared_ptr<IRequestDelegate> createRequestDelegate() override;

    /// @see Super.
    virtual void
    shareLinkContent(const std::string& url,
                     const std::shared_ptr<IShareDelegate>& delegate) override;

    /// @see Super.
    virtual void
    sharePhotoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) override;

    /// @see Super.
    virtual void
    shareVideoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) override;

    /// @see Super.
    virtual std::shared_ptr<IShareDelegate> createShareDelegate() override;

protected:
    void registerNotifications();
    void onProfileChanged(const std::string& profile);

private:
    int delegateId_;
    IMessageBridge& bridge_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_ */
