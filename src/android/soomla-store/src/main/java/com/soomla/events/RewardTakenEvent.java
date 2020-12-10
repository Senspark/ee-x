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

package com.soomla.events;


/**
 *  This event is fired when a <code>Reward</code> has been taken.
 */
public class RewardTakenEvent extends SoomlaEvent {

    /** read-only Properties **/

    public final String RewardId;

    /**
     * Constructor
     *
     * @param rewardId the reward that was taken
     */
    public RewardTakenEvent(String rewardId) {
        this(rewardId, null);
    }

    public RewardTakenEvent(String rewardId, Object sender) {
        super(sender);
        RewardId = rewardId;
    }
}
