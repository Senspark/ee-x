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
    /// Used by JSB.
    Bridge();

    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual bool isLoggedIn() const override;
    virtual void
    logIn(const std::vector<std::string>& permissions,
          const std::shared_ptr<ILoginDelegate>& delegate) override;
    virtual std::shared_ptr<ILoginDelegate> createLoginDelegate() override;
    virtual void logOut() override;
    virtual std::shared_ptr<IAccessToken> getAccessToken() const override;
    virtual void
    graphRequest(const GraphRequest& request,
                 const std::shared_ptr<IGraphDelegate>& delegate) override;
    virtual std::shared_ptr<IGraphDelegate> createGraphDelegate() override;
    virtual void
    sendRequest(const RequestContent& content,
                const std::shared_ptr<IRequestDelegate>& delegate) override;
    virtual std::shared_ptr<IRequestDelegate> createRequestDelegate() override;
    virtual void
    shareLinkContent(const std::string& url,
                     const std::shared_ptr<IShareDelegate>& delegate) override;
    virtual void
    sharePhotoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) override;
    virtual void
    shareVideoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) override;
    virtual std::shared_ptr<IShareDelegate> createShareDelegate() override;

protected:
    void registerNotifications();
    void onProfileChanged(const std::string& profile);

private:
    int delegateId_;
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_ */
