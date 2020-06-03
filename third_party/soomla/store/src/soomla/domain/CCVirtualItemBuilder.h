//
//  CCVirtualItemBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef SOOMLA_VIRTUAL_ITEM_BUILDER_HPP
#define SOOMLA_VIRTUAL_ITEM_BUILDER_HPP

#ifdef __cplusplus

#include <optional>
#include <string>

#include "soomla/domain/CCVirtualItem.h"

namespace soomla {
template <class T>
class VirtualItemBuilder {
private:
    using Self = VirtualItemBuilder;

public:
    VirtualItemBuilder() = default;
    ~VirtualItemBuilder() = default;

    T& setName(const std::string& name);
    T& setDescription(const std::string& description);
    T& setItemId(const std::string& itemId);

    virtual CCVirtualItem* build() const = 0;

protected:
    std::string name_; // Optional.
    std::string description_; // Optional.
    std::optional<std::string> itemId_;
};
} // namespace soomla

#endif // __cplusplus

#endif /* SOOMLA_VIRTUAL_ITEM_BUILDER_HPP */
