//
//  CCStoreAssetsBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/12/18.
//

#include "CCStoreAssetsBuilder.h"

namespace soomla {

using StoreAssets_ = CCStoreAssetsBuilder::StoreAssets;
using Self = CCStoreAssetsBuilder;

std::int32_t StoreAssets_::getVersion() {
    return version_;
}

cocos2d::Vector<soomla::CCVirtualCurrency*>& StoreAssets_::getCurrencies() {
    return currencies_;
}

cocos2d::Vector<soomla::CCVirtualGood*>& StoreAssets_::getGoods() {
    return goods_;
}

cocos2d::Vector<soomla::CCVirtualCurrencyPack*>&
StoreAssets_::getCurrencyPacks() {
    return currencyPacks_;
}

cocos2d::Vector<soomla::CCVirtualCategory*>& StoreAssets_::getCategories() {
    return categories_;
}

CCStoreAssetsBuilder::CCStoreAssetsBuilder() {
    storeAssets_ = cocos2d::RefPtr<StoreAssets_>(new StoreAssets_());
}

soomla::CCStoreAssets* CCStoreAssetsBuilder::build() {
    return storeAssets_;
}

Self& CCStoreAssetsBuilder::setVersion(std::int32_t version) {
    storeAssets_->version_ = version;
    return *this;
}

Self& CCStoreAssetsBuilder::addCurrency(soomla::CCVirtualCurrency* currency) {
    storeAssets_->currencies_.pushBack(currency);
    return *this;
}

Self& CCStoreAssetsBuilder::addGood(soomla::CCVirtualGood* good) {
    storeAssets_->goods_.pushBack(good);
    return *this;
}

Self& CCStoreAssetsBuilder::addCurrencyPack(
    soomla::CCVirtualCurrencyPack* pack) {
    storeAssets_->currencyPacks_.pushBack(pack);
    return *this;
}

Self& CCStoreAssetsBuilder::addCategorie(soomla::CCVirtualCategory* category) {
    storeAssets_->categories_.pushBack(category);
    return *this;
}
} // namespace soomla
