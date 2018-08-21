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

#import "VirtualItem.h"

/**
 This is a representation of a game's virtual currency.
 Each game can have multiple instances of a virtual currency, all kept in 
 `StoreInfo`.
 
 Real Game Examples: 'Coin', 'Gem', 'Muffin'
 
 NOTE: This item is NOT purchasable!
 However, a `VirtualCurrencyPack` IS purchasable.
 For example, if the virtual currency in your game is a 'Coin' and you want 
 to make a single 'Coin' available for purchase you will need to define a 
 `VirtualCurrencyPack` of 1 'Coin'.
 */
@interface VirtualCurrency : VirtualItem {
    
}

/** 
 Constructor
 
 @param name see parent.
 @param description see parent.
 @param itemId see parent.
 */
- (id)initWithName:(NSString*)name andDescription:(NSString*)description
    andItemId:(NSString*)itemId;

/** 
 Constructor
 
 @param dict see parent.
 */
- (id)initWithDictionary:(NSDictionary*)dict;

/**
 see parent.
 
 @return see parent.
 */
- (NSDictionary*)toDictionary;

@end
