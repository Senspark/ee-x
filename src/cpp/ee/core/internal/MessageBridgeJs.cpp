#include "ee/core/internal/MessageBridgeJs.hpp"

#include "ee/core/JsbUtils.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Task.hpp"
#include "ee/core/internal/MessageBridgeUtils.hpp"

namespace ee {
namespace core {
namespace {
bool ee_staticCall_binder(se::State& state) {
    constexpr std::size_t Arity = 2;
    auto&& args = state.args();
    if (Arity != args.size()) {
        SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                        args.size(), Arity);
        return false;
    }
    se::Value jsThis(state.thisObject());
    auto result =
        MessageBridgeUtils::call(args[0].toString(), args[1].toString());
    state.rval().setString(result);
    return true;
}

constexpr auto ee_staticCall = &ee_staticCall_binder;

SE_BIND_FUNC(ee_staticCall)

bool registerJsb(se::Object* global) {
    global->defineFunction("ee_staticCall", _SE(ee_staticCall));
}
} // namespace

using Self = MessageBridgeJs;

Self::MessageBridgeJs() {
    auto&& engine = se::ScriptEngine::getInstance();
    engine->addRegisterCallback(registerJsb);
}

Self::~MessageBridgeJs() = default;

void Self::registerHandler(const MessageHandler& handler,
                           const std::string& tag) {
    throw std::runtime_error("Operation not supported");
}

void Self::deregisterHandler(const std::string& tag) {
    throw std::runtime_error("Operation not supported");
}

std::string Self::call(const std::string& tag, const std::string& message) {
    throw std::runtime_error("Operation not supported");
}

void Self::callCpp(const std::string& tag, const std::string& message) {
    // FIXME.
}

Task<std::string> Self::callAsync(const std::string& tag,
                                  const std::string& message) {
    throw std::runtime_error("Operation not supported");
}
} // namespace core
} // namespace ee
