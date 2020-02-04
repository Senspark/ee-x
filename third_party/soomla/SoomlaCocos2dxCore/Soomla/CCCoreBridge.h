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

#ifndef __CCCoreBridge_H_
#define __CCCoreBridge_H_

#include <cocos2d.h>

#include "Soomla/CCError.h"
#include "Soomla/rewards/CCReward.h"
#include "Soomla/rewards/CCSequenceReward.h"

namespace soomla {

    class CCCoreBridge: public cocos2d::Ref {
    public:
		/**
		   This class is singleton, use this function to access it.
		*/
        static CCCoreBridge* getInstance();

        static void initShared();

        CCCoreBridge() = default;

        virtual bool init();
    };
};

#endif // !__CCCoreBridge_H_
