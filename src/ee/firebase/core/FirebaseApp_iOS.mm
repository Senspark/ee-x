//
//  FirebaseApp.m
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/core/FirebaseApp.hpp"

#include <firebase/app.h>

namespace ee {
namespace firebase {
using Self = App;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
    auto app = ::firebase::App::Create(options);
    static_cast<void>(app);

    initialized = true;
}
} // namespace firebase
} // namespace ee
