//
//  FirebaseApp.hpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_CORE_HPP
#define EE_X_FIREBASE_CORE_HPP

#include "ee/FirebaseFwd.hpp"

namespace ee {
namespace firebase {
class Core {
public:
    /// Initializes the Firebase app.
    static void initialize();
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_CORE_HPP */
