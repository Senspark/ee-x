#include "ee/core/internal/ThreadImplCpp.hpp"

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace ee {
namespace core {
using Self = ThreadImplCpp;

void Self::initialize() {
    id_ = std::this_thread::get_id();
}

bool Self::isLibraryThread() const {
    return id_ == std::this_thread::get_id();
}

bool Self::runOnLibraryThread(const std::function<void()>& runnable) {
    if (id_ == std::this_thread::get_id()) {
        runnable();
        return true;
    }
    auto director = cocos2d::Director::getInstance();
    auto scheduler = director->getScheduler();
    scheduler->performFunctionInCocosThread([runnable] { //
        runnable();
    });
    return false;
}
} // namespace core
} // namespace ee

#endif // EE_X_COCOS_CPP