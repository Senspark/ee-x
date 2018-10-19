//
//  jsb_admob.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#ifndef JSB_ADMOB_BRIDGE_HPP__
#define JSB_ADMOB_BRIDGE_HPP__

namespace se {
class Object;
} // namespace se

namespace ee {
namespace admob {

extern se::Object* __admobObj;

bool register_admob_bridge_manual(se::Object* object);

} // namespace admob
} // namespace ee

#endif /* JSB_ADMOB_HPP__ */
