/*
 * Copyright (C) 2012-2014 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.soomla.data;

/**
 * This class contains all static final String names of the keys/vals in the JSON being parsed all
 * around the sdk.
 */
public class JSONConsts {

    /** Global **/
    public static final String SOOM_ENTITY_NAME         = "name";
    public static final String SOOM_ENTITY_DESCRIPTION  = "description";
    public static final String SOOM_ENTITY_ID           = "itemId";
    public static final String SOOM_CLASSNAME           = "className";
    public static final String SOOM_SCHEDULE            = "schedule";


    /** Reward **/

    public static final String SOOM_REWARDS             = "rewards";
    public static final String SOOM_REWARD_ICONURL      = "iconUrl";


    /** Schedule **/
    public static final String SOOM_SCHE_REC            = "schedRecurrence";
    public static final String SOOM_SCHE_RANGES         = "schedTimeRanges";
    public static final String SOOM_SCHE_RANGE_START    = "schedTimeRangeStart";
    public static final String SOOM_SCHE_RANGE_END      = "schedTimeRangeEnd";
    public static final String SOOM_SCHE_APPROVALS      = "schedApprovals";

}

