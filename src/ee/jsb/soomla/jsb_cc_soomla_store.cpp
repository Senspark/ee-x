#include "ee/jsb/core/jsb_core_common.hpp"

#include "Cocos2dxStore.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_soomla_store.hpp"

namespace ee {
namespace core {

template <>
soomla::CCStoreAssets* get_value(const se::Value& value) {
    return static_cast<soomla::CCStoreAssets*>(
        value.toObject()->getPrivateData());
}

} // namespace core
} // namespace ee

namespace soomla {

static se::Object* __jsb_CCSoomlaStore_proto = nullptr;
static se::Class* __jsb_CCSoomlaStore_class = nullptr;

const static auto jsb_CCSoomlaStore_initialize =
    &ee::core::jsb_static_call<&CCSoomlaStore::initialize, CCStoreAssets*,
                               cocos2d::ValueMap>;

SE_BIND_FUNC(jsb_CCSoomlaStore_initialize)

bool register_cc_soomla_store_manual(se::Object* globalObj) {
    se::Object* soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &soomlaObj);

    auto cls = se::Class::create("CCSoomlaStore", soomlaObj, nullptr, nullptr);

    cls->install();

    JSBClassType::registerClass<soomla::CCSoomlaStore>(cls);

    __jsb_CCSoomlaStore_proto = cls->getProto();
    __jsb_CCSoomlaStore_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (soomlaObj->getProperty("CCSoomlaStore", &ctorVal) &&
        ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("initialize",
                                           _SE(jsb_CCSoomlaStore_initialize));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
