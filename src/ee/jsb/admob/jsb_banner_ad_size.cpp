//
//  jsb_banner_ad_size.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "jsb_banner_ad_size.hpp"

#include "jsb_admob_bridge.hpp"
#include "jsb_core_common.hpp"

#include <ee/Admob.hpp>

#include <unordered_map>

namespace ee {
namespace admob {
static se::Object* __jsb_BannerAdSize_proto = nullptr;
static se::Class* __jsb_BannerAdSize_class = nullptr;
static std::unordered_map<BannerAdSize, se::Object*> __s_bannerSizes;
} // namespace admob

namespace core {
template <> ee::AdMobBannerAdSize get_value(const se::Value& value) {
    return *static_cast<ee::AdMobBannerAdSize*>(
        value.toObject()->getPrivateData());
}

template <> void set_value(se::Value& value, ee::admob::BannerAdSize input) {
    se::Object* obj = nullptr;
    if (admob::__s_bannerSizes.count(input) == 0) {
        obj =
            se::Object::createObjectWithClass(admob::__jsb_BannerAdSize_class);
        obj->setPrivateData(new admob::BannerAdSize(input));
    } else {
        obj = admob::__s_bannerSizes[input];
    }
    value.setObject(obj);
}
} // namespace core

namespace admob {

template <ee::AdMobBannerAdSize bannerSize>
ee::AdMobBannerAdSize getBannerSize() {
    return bannerSize;
}

const static auto jsb_BannerAdSize_finalize =
    &ee::core::jsb_finalize<ee::AdMobBannerAdSize>;
const static auto jsb_BannerAdSize_constructor =
    &ee::core::jsb_constructor<ee::AdMobBannerAdSize>;
const static auto jsb_BannerAdSize_Normal =
    &ee::core::jsb_static_get<ee::AdMobBannerAdSize,
                              &getBannerSize<ee::AdMobBannerAdSize::Normal>>;
const static auto jsb_BannerAdSize_Large =
    &ee::core::jsb_static_get<ee::AdMobBannerAdSize,
                              &getBannerSize<ee::AdMobBannerAdSize::Large>>;
const static auto jsb_BannerAdSize_Smart =
    &ee::core::jsb_static_get<ee::AdMobBannerAdSize,
                              &getBannerSize<ee::AdMobBannerAdSize::Smart>>;

SE_BIND_FINALIZE_FUNC(jsb_BannerAdSize_finalize)
SE_BIND_CTOR(jsb_BannerAdSize_constructor, __jsb_BannerAdSize_class,
             jsb_BannerAdSize_finalize)
SE_BIND_FUNC(jsb_BannerAdSize_Normal)
SE_BIND_FUNC(jsb_BannerAdSize_Large)
SE_BIND_FUNC(jsb_BannerAdSize_Smart)

bool register_banner_ad_size_manual(se::Object* globalObj) {
    auto cls = se::Class::create("BannerAdSize", __admobObj, nullptr,
                                 _SE(jsb_BannerAdSize_constructor));
    cls->defineFinalizeFunction(_SE(jsb_BannerAdSize_finalize));

    cls->install();

    JSBClassType::registerClass<ee::AdMobBannerAdSize>(cls);

    __jsb_BannerAdSize_proto = cls->getProto();
    __jsb_BannerAdSize_class = cls;

    se::Value ctorVal;
    if (__admobObj->getProperty("BannerAdSize", &ctorVal) &&
        ctorVal.isObject()) {
        ctorVal.toObject()->defineProperty(
            "Normal", _SE(jsb_BannerAdSize_Normal), nullptr);
        ctorVal.toObject()->defineProperty("Large", _SE(jsb_BannerAdSize_Large),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Smart", _SE(jsb_BannerAdSize_Smart),
                                           nullptr);
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
