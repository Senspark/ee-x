//
//  jsb_cc_ifacebook_access_token.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef jsb_cc_ifacebook_access_token_hpp
#define jsb_cc_ifacebook_access_token_hpp

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace core {
template <typename T>
se::Object* create_JSON_object(const T& value);
}
namespace facebook {
se::Class* getIAccessTokenClass();
bool register_ifacebook_access_token_manual(se::Object* object);
} // namespace facebook
} // namespace ee

#endif /* jsb_cc_ifacebook_access_token_hpp */
