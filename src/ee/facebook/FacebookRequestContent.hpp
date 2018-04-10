//
//  FacebookRequest.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#ifndef EE_X_FACEBOOK_REQUEST_CONTENT_HPP
#define EE_X_FACEBOOK_REQUEST_CONTENT_HPP

#include <string>
#include <vector>

#include "ee/FacebookFwd.hpp"

namespace ee {
namespace facebook {
class RequestContent {
private:
    using Self = RequestContent;

public:
    enum class ActionType {
        /// No action type.
        None = 0,

        /// Send action type: The user is sending an object to the friends.
        Send,

        /// Ask For action type: The user is asking for an object from friends.
        AskFor,

        /// Turn action type: It is the turn of the friends to play against the
        /// user in a match. (no object)
        Turn
    };

    enum class Filter {
        /// No filter, all friends can be displayed.
        None = 0,

        /// Friends using the app can be displayed.
        AppUsers,

        /// Friends not using the app can be displayed.
        AppNonUsers
    };

    RequestContent();

    Self& setActionType(ActionType type);
    Self& setFilter(Filter filter);
    Self& setRecipients(const std::vector<std::string>& recipients);
    Self& setObjectId(const std::string& objectId);
    Self& setTitle(const std::string& title);
    Self& setMessage(const std::string& message);
    Self& setData(const std::string& data);
    
    std::string toString() const;

private:
    friend Bridge;

    ActionType actionType_;
    Filter filter_;
    std::vector<std::string> recipients_;
    std::string objectId_;
    std::string title_;
    std::string message_;
    std::string data_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_REQUEST_CONTENT_HPP */
