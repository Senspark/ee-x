//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "jsb_interstitial_ad.hpp"

#include "jsb_core_common.hpp"

#include <ee/Ads.hpp>

#include <unordered_map>

namespace ee {
namespace ads {
static se::Object* __jsb_InterstitialAd_proto = nullptr;
static se::Class* __jsb_InterstitialAd_class = nullptr;

static std::unordered_map<std::shared_ptr<ee::IInterstitialAd>, se::Object*>
    __jsb_s_interstitialAds;
static std::vector<std::shared_ptr<ee::IInterstitialAd>>
    __jsb_s_interstitialAdArchive;
} // namespace ads

namespace core {
template <>
void set_value(se::Value& value, std::shared_ptr<ee::IInterstitialAd> input) {
    se::Object* obj = nullptr;
    if (ads::__jsb_s_interstitialAds.count(input) != 0) {
        obj = ads::__jsb_s_interstitialAds.at(input);
    } else {
        ads::__jsb_s_interstitialAdArchive.push_back(input);
        obj = se::Object::createObjectWithClass(ads::__jsb_InterstitialAd_class);
        obj->setPrivateData(input.get());
        obj->root();
    }
    value.setObject(obj);
}

namespace ads {

} // namespace ads
} // namespace core
} // namespace ee
