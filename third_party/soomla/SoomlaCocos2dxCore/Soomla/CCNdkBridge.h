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

#ifndef __CCNdkBridge_H_
#define __CCNdkBridge_H_

#include <cocos2d.h>

#include "Soomla/CCError.h"

namespace soomla {
/** \class CCNdkBridge
    \brief The bridge between the application and the Cocos2d-x code.

    This class receives calls from the native application and sends them
    over to the Cocos2d-x code, and vice versa.
 */
class CCNdkBridge {
public:
    /**
       Call a native application function.
       \param params A __Dictionary containing the name of the function to call
       and parameters to pass to it. \param pError A CCError that can be used to
       tell if there was an error. \return The return value of the called
       function as a cocos2d::Ref.
     */

    static cocos2d::ValueMap callNative(const cocos2d::ValueMap& params,
                                        CCError** pError);
};
}; // namespace soomla

#endif //__CCNdkBridge_H_
