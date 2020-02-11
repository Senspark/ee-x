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

//  Created by Igor Yegoroff on 5/16/13.

#include "soomla/domain/CCVirtualItem.h"

#include "soomla/data/CCStoreInfo.h"

namespace soomla {

    using namespace cocos2d;

    const std::string& CCVirtualItem::getItemId() {
        return getId();
    }
    
    void CCVirtualItem::save(bool saveToDB) {
        CCStoreInfo::sharedStoreInfo()->saveItem(this, saveToDB);
    }
}
