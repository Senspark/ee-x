//
//  IFacebookRequestDelegate.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#ifndef EE_X_I_FACEBOOK_DELEGATE_HPP
#define EE_X_I_FACEBOOK_DELEGATE_HPP

#include <functional>
#include <string>

#include "ee/FacebookFwd.hpp"

namespace ee {
namespace facebook {
template <std::size_t Id, class... Args>
class IDelegate {
private:
    using Self = IDelegate;

public:
    using SuccessCallback = std::function<void(Args... args)>;
    using FailureCallback = std::function<void(const std::string& message)>;
    using CancelCallback = std::function<void()>;

    virtual ~IDelegate() = default;

    virtual Self& onSuccess(const SuccessCallback& callback) = 0;
    virtual Self& onFailure(const FailureCallback& callback) = 0;
    virtual Self& onCancel(const CancelCallback& callback) = 0;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_I_FACEBOOK_REQUEST_DELEGATE_HPP */
