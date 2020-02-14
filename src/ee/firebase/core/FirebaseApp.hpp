//
//  FirebaseApp.hpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_APP_HPP_
#define EE_X_FIREBASE_APP_HPP_

#include "ee/FirebaseFwd.hpp"

namespace ee {
namespace firebase {
class App {
public:
    /// Initializes the Firebase app.
    static void initialize();
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_APP_HPP_ */
