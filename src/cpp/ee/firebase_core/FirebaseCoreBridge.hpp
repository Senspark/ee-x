//
//  FirebaseApp.hpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_CORE_BRIDGE_HPP
#define EE_X_FIREBASE_CORE_BRIDGE_HPP

#ifdef __cplusplus

#include "ee/FirebaseFwd.hpp"

namespace ee {
namespace firebase {
namespace core {
class Bridge {
public:
    /// Initializes the Firebase app.
    static void initialize();
};
} // namespace core
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_CORE_BRIDGE_HPP */
