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

#import "PurchaseType.h"

@class VirtualItem;

/**
 This type of purchase allows users to purchase a virtual item with other
 virtual items.
 
 Real Game Example: Purchase a 'Sword' in exchange for 100 'Gem's. 'Sword' is 
 the item to be purchased, 'Gem' is the target item, and 100 is the amount.
 */
@interface PurchaseWithVirtualItem : PurchaseType {
    NSString* targetItemId;
    int amount;
}

@property (retain, nonatomic) NSString* targetItemId;
@property (nonatomic) int amount;

/**
 Constructor
 
 @param oTargetItemId - The itemId of the `VirtualItem` that is used to "pay"
    in order to make the purchase.
 @param oAmount - The number of target items needed in order to make the 
    purchase.
 */
- (id) initWithVirtualItem:(NSString*)oTargetItemId andAmount:(int)oAmount;


@end
