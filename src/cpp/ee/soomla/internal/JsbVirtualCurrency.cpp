#include "ee/soomla/internal/JsbVirtualCurrency.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/virtualCurrencies/CCVirtualCurrency.h>

#include "ee/soomla/internal/jsb_cc_virtual_item.hpp"

namespace soomla {
namespace {
constexpr auto finalize = &ee::JsbUtils::makeFinalize<CCVirtualCurrency>;

SE_BIND_FINALIZE_FUNC(finalize)
} // namespace

bool registerVirtualCurrency(se::Object* global) {
    auto ns = ee::JsbUtils::getPath(global, "soomla");
    auto virtualItemProto = getCCVirtualItemProto();
    auto cls =
        se::Class::create("CCVirtualCurrency", ns, virtualItemProto, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();
    JSBClassType::registerClass<CCVirtualCurrency>(cls);
    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS