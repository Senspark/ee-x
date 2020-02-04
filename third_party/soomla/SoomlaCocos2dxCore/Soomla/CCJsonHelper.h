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

#ifndef __CCJsonHelper_H_
#define __CCJsonHelper_H_

#include <iostream>

#include <jansson.h>

#include <cocos2d.h>

/** \class CCJsonHelper
    \brief A helper class to generate CCObjects from JSON objects and vice
   versa.

    A helper class to generate CCObjects from JSON strings and vice versa.
 */
class CCJsonHelper {
public:

    static cocos2d::Value getValueFromJson(json_t* obj);
    
    static json_t* getJsonFromValue(const cocos2d::Value& value);
};

#endif //__CCJsonHelper_H_
