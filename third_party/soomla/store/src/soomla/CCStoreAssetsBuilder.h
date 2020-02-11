//
//  CCStoreAssetsBuilder.h
//  Pods
//
//  Created by Le Van Kiet on 9/12/18.
//

#ifndef CCStoreAssetsBuilder_h
#define CCStoreAssetsBuilder_h

#include "soomla/CCStoreAssets.h"

namespace soomla {

class CCVirtualCurrency;
class CCVirtualGood;
class CCVirtualCurrencyPack;
class CCVirtualCategory;

class CCStoreAssetsBuilder {
    using Self = CCStoreAssetsBuilder;

public:
    class StoreAssets : public soomla::CCStoreAssets {
    public:
        virtual std::int32_t getVersion() override;
        /**
        @see parent
        */
        virtual cocos2d::Vector<soomla::CCVirtualCurrency*>&
        getCurrencies() override;

        /**
        @see parent
        */
        virtual cocos2d::Vector<soomla::CCVirtualGood*>& getGoods() override;

        /**
        @see parent
        */
        virtual cocos2d::Vector<soomla::CCVirtualCurrencyPack*>&
        getCurrencyPacks() override;

        /**
        @see parent
        */
        virtual cocos2d::Vector<soomla::CCVirtualCategory*>&
        getCategories() override;

    public:
        cocos2d::Vector<soomla::CCVirtualCurrency*> currencies_;
        cocos2d::Vector<soomla::CCVirtualGood*> goods_;
        cocos2d::Vector<soomla::CCVirtualCurrencyPack*> currencyPacks_;
        cocos2d::Vector<soomla::CCVirtualCategory*> categories_;
        std::int32_t version_;
    };

    CCStoreAssetsBuilder();

    Self& setVersion(std::int32_t version);

    Self& addCurrency(soomla::CCVirtualCurrency* currency);

    Self& addGood(soomla::CCVirtualGood* good);

    Self& addCurrencyPack(soomla::CCVirtualCurrencyPack* pack);

    Self& addCategorie(soomla::CCVirtualCategory* category);

    soomla::CCStoreAssets* build();

private:
    cocos2d::RefPtr<StoreAssets> storeAssets_;
};
} // namespace soomla

#endif /* CCStoreAssetsBuilder_h */
