//
//  CCVirtualCurrencyBuilder.hpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#ifndef CCVirtualCurrencyBuilder_hpp
#define CCVirtualCurrencyBuilder_hpp

#include "CCVirtualCurrency.h"

namespace soomla {

class CCVirtualCurrencyBuilder {
private:
    using Self = CCVirtualCurrencyBuilder;

public:
    CCVirtualCurrencyBuilder();
    ~CCVirtualCurrencyBuilder();

    Self& setName(const std::string& name);

    Self& setDescription(const std::string& description);

    Self& setItemId(const std::string& itemId);

    CCVirtualItem* build();

protected:
    std::unique_ptr<std::string> name_;
    std::unique_ptr<std::string> description_;
    std::unique_ptr<std::string> itemId_;
};

} // namespace soomla

#endif /* CCVirtualCurrencyBuilder_hpp */
