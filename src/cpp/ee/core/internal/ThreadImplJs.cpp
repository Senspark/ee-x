#include "ee/core/internal/ThreadImplJs.hpp"

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS
#include <base/CCScheduler.h>
#include <platform/CCApplication.h>

namespace ee {
namespace core {
using Self = ThreadImplJs;

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
    auto scheduler = cocos2d::Application::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([runnable] { //
        runnable();
    });
    return false;
}
} // namespace core
} // namespace ee

#endif // EE_X_COCOS_JS