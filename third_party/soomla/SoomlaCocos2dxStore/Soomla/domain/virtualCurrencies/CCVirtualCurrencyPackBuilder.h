//
//  CCVirtualCurrencyPackBuilder.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#ifndef CCVirtualCurrencyPackBuilder_hpp
#define CCVirtualCurrencyPackBuilder_hpp

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

namespace soomla {

class CCVirtualCurrencyPackBuilder {
private:
    using Self = CCVirtualCurrencyPackBuilder;

public:
    CCVirtualCurrencyPackBuilder();
    ~CCVirtualCurrencyPackBuilder();

    Self& setName(const std::string& name);

    Self& setDescription(const std::string& description);

    Self& setItemId(const std::string& itemId);

    Self& setCurrencyAmount(int currencyAmount);

    Self& setCurrencyItemId(const std::string& currencyItemId);

    Self& setPurchaseType(CCPurchaseType* type);

    CCVirtualCurrencyPack* build();

protected:
    std::unique_ptr<std::string> name_;
    std::unique_ptr<std::string> description_;
    std::unique_ptr<std::string> itemId_;
    std::unique_ptr<int> currencyAmount_;
    std::unique_ptr<std::string> currencyItemId_;
    cocos2d::RefPtr<soomla::CCPurchaseType> type_;
};

} // namespace soomla

#endif /* CCVirtualCurrencyPackBuilder_hpp */
