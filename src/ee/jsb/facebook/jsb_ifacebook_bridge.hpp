//
//  jsb_ifacebook_bridge.hpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#ifndef jsb_ifacebook_bridge_hpp
#define jsb_ifacebook_bridge_hpp

#include "ee/jsb/jsb_fwd.hpp"

namespace ee {
namespace facebook {
se::Class* getIFacebookBridgeClass();
bool register_ifacebook_bridge_manual(se::Object* object);
} // namespace facebook
} // namespace ee

#endif /* jsb_ifacebook_bridge_hpp */
