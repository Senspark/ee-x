//
//  jsb_facebook_bridge.hpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#ifndef jsb_facebook_ads_bridge_hpp
#define jsb_facebook_ads_bridge_hpp

namespace se {
class Object;
} // namespace se

namespace ee {
namespace facebook {

extern se::Object* __facebookAdObj;

bool register_facebookads_bridge_manual(se::Object* object);

} // namespace facebook
} // namespace ee

#endif /* jsb_facebook_bridge_hpp */
