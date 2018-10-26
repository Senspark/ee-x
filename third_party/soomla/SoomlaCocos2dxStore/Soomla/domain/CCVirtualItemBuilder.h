//
//  CCVirtualItemBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef CCVirtualItemBuilder_hpp
#define CCVirtualItemBuilder_hpp

#include "CCVirtualItem.h"

namespace soomla {

class CCVirtualItemBuilder {
private:
    using Self = CCVirtualItemBuilder;

public:
    class VirtualItem : public CCVirtualItem {
    public:
        std::unique_ptr<std::string> name_;
        std::unique_ptr<std::string> description_;
        std::unique_ptr<std::string> itemId_;
    };

public:
    CCVirtualItemBuilder();
    ~CCVirtualItemBuilder();

    Self& setName(const std::string& name);

    Self& setDescription(const std::string& description);

    Self& setItemId(const std::string& itemId);

    CCVirtualItem* build();

private:
    std::unique_ptr<VirtualItem> virtualItem_;
};

} // namespace soomla

#endif /* CCVirtualItemBuilder_hpp */
