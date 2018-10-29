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

#ifndef __CCNativeStoreInfo_H_
#define __CCNativeStoreInfo_H_

#include "CCStoreInfo.h"

namespace soomla {
    /**
     Implements the `CCStoreInfo` using the bridge to talk
     with the native implementation of StoreInfo
     
     See parent for all functions.
     */
    class CCNativeStoreInfo : public CCStoreInfo {
    public:
        CCNativeStoreInfo() : CCStoreInfo() {}
        
        virtual ~CCNativeStoreInfo();
        
        virtual void save();
    protected:
        virtual void setStoreAssets(CCStoreAssets *storeAssets);
    };
}

#endif // __CCNativeStoreInfo_H_
