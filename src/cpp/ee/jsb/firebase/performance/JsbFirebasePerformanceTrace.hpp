//
//  JsbFirebasePerformanceTrace.hpp
//  ee-x
//
//  Created by eps on 3/25/19.
//

#ifndef EE_X_JSB_FIREBASE_PERFORMANCE_TRACE_HPP
#define EE_X_JSB_FIREBASE_PERFORMANCE_TRACE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace firebase {
namespace performance {
bool registerJsbTrace(se::Object* global);
} // namespace performance
} // namespace firebase
} // namespace ee

#endif /* EE_X_JSB_FIREBASE_PERFORMANCE_TRACE_HPP */
