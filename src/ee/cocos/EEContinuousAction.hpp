//
//  EEContinuousAction.hpp
//  ee-library
//
//  Created by Zinge on 5/12/17.
//
//

#ifndef EE_LIBRARY_CONTINUOUS_ACTION_HPP_
#define EE_LIBRARY_CONTINUOUS_ACTION_HPP_

#include <functional>
#include <vector>

namespace ee {
class ContinuousAction {
public:
    using Action = std::function<void()>;
    using TailAction = std::function<void(const Action& action)>;

    ContinuousAction& addAction(const TailAction& tailAction);

    Action build() const;

private:
    std::vector<TailAction> actions_;
};
} // namespace ee

#endif /* EE_LIBRARY_CONTINUOUS_ACTION_HPP_ */
