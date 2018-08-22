/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __CCVirtualCategory_H_
#define __CCVirtualCategory_H_

#include "CCDomain.h"
#include "CCSoomlaMacros.h"
#include "CCStoreConsts.h"
#include "cocos2d.h"

namespace soomla {
/**
 @class CCVirtualCategory
 @brief A category for virtual items.

 This class is a definition of a category. A single category can be
 associated with many virtual goods. Virtual categories help in organizing
 your economy's virtual goods.
 */
class CCVirtualCategory : public CCDomain {
public:
    CCVirtualCategory()
        : mName(nullptr) {};

    /**
     Creates a virtual category.
     @param name The name of the category.
     @param goodItemIds An array containing the item ids of all the items
            in this category.
     @return The virtual category.
     */
    static CCVirtualCategory*
    create(const std::string& name,
           const std::vector<std::string>& goodItemIds);

    /**
     Creates a virtual category.
     @param dict A dictionary containing keys to each of the parameters
            required by the create function.
     @return The virtual category.
    */
    SL_CREATE_WITH_VALUE_MAP(CCVirtualCategory);

    virtual bool init(const std::string& name,
                      const std::vector<std::string>& goodItemIds);
    virtual bool initWithValueMap(const cocos2d::ValueMap& map) override;

    /**
    Converts this `~CCVirtualCategory` to a `CCDictionary`.
    @return `CCDictionary` representation of this `~CCVirtualCategory`.
    */
    virtual cocos2d::ValueMap toValueMap() override;

    virtual ~CCVirtualCategory() = default;

    CC_SYNTHESIZE_PASS_BY_REF(std::string, mName, Name);
    CC_SYNTHESIZE_PASS_BY_REF(std::vector<std::string>, mGoodItemIds,
                              GoodItemIds);
                              
protected:
    void fillNameFromValueMap(const cocos2d::ValueMap& map);
    void putNameToValueMap(cocos2d::ValueMap& map);
    void fillGoodItemIdsFromValueMap(const cocos2d::ValueMap& map);
    void putGoodItemIdsToValueMap(cocos2d::ValueMap& map);

};
}; // namespace soomla

#endif //__CCVirtualCategory_H_
