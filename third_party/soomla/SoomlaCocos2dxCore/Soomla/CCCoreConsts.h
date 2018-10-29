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

#ifndef __CCCoreConsts__
#define __CCCoreConsts__

namespace soomla {
    class CCCoreConsts {
    public:
        static char const *JSON_ITEM_NAME;
        static char const *JSON_ITEM_DESCRIPTION;
        static char const *JSON_ITEM_ITEM_ID;

        static char const* JSON_JSON_TYPE;

        static char const *JSON_SCHEDULE;
        static char const *JSON_ICON_URL;
        static char const *JSON_REWARDS;

        /** Schedule **/
        static char const *JSON_SCHE_REC;
        static char const *JSON_SCHE_RANGES;
        static char const *JSON_SCHE_RANGE_START;
        static char const *JSON_SCHE_RANGE_END;
        static char const *JSON_SCHE_APPROVALS;

        static char const *JSON_JSON_TYPE_BADGE;
        static char const *JSON_JSON_TYPE_RANDOM;
        static char const *JSON_JSON_TYPE_SEQUENCE;
        static char const *JSON_JSON_TYPE_SCHEDULE;
        static char const *JSON_JSON_TYPE_DATE_TIME_RANGE;
        
        /** Events **/
        
        static char const *DICT_ELEMENT_REWARD;
        static char const *DICT_ELEMENT_NAME;
        static char const *DICT_ELEMENT_EXTRA;

        static char const *EVENT_REWARD_GIVEN;
        static char const *EVENT_REWARD_TAKEN;
        static char const *EVENT_CUSTOM;
    };
}

#endif /* defined(__CCCoreConsts__) */
