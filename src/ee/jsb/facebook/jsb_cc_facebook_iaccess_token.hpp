//
//  jsb_cc_facebook_iaccess_token.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef jsb_cc_facebook_iaccess_token_hpp
#define jsb_cc_facebook_iaccess_token_hpp

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace facebook {
    
    se::Class* getIAccessTokenClass();
    
bool register_facebook_iaccess_token_manual(se::Object* object);
}
} // namespace ee

#endif /* jsb_cc_facebook_iaccess_token_hpp */
