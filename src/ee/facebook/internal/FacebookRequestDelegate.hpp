//
//  FacebookRequestDelegate.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#ifndef EE_X_FACEBOOK_REQUEST_DELEGATE_HPP
#define EE_X_FACEBOOK_REQUEST_DELEGATE_HPP

#include <memory>

#include "ee/CoreFwd.hpp"
#include "ee/facebook/IFacebookDelegate.hpp"

namespace ee {
namespace facebook {
class RequestDelegate : public IRequestDelegate {
private:
    using Self = RequestDelegate;

public:
    explicit RequestDelegate(IMessageBridge& bridge, int tag);

    virtual ~RequestDelegate() override;

    /// @see Super.
    virtual Self& onSuccess(const SuccessCallback& callback) override;

    /// @see Super.
    virtual Self& onFailure(const FailureCallback& callback) override;

    /// @see Super.
    virtual Self& onCancel(const CancelCallback& callback) override;

protected:
    friend Bridge;

private:
    IMessageBridge& bridge_;
    int tag_;
    SuccessCallback successCallback_;
    FailureCallback failureCallback_;
    CancelCallback cancelCallback_;
    std::shared_ptr<Self> self_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_REQUEST_DELEGATE_HPP */
