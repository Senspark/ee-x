//
//  FirebaseDynamicLink.hpp
//  ee_x_test
//
//  Created by eps on 5/16/18.
//

#ifndef EE_X_TEST_FIREBASE_DYNAMIC_LINK_HPP
#define EE_X_TEST_FIREBASE_DYNAMIC_LINK_HPP

#include <memory>

#include <ee/FirebaseFwd.hpp>

namespace eetest {
class FirebaseDynamicLink {
public:
    FirebaseDynamicLink();

private:
    std::unique_ptr<ee::FirebaseDynamicLink> plugin_;
};
} // namespace eetest

#endif /* EE_X_FIREBASE_DYNAMIC_LINK_HPP */
