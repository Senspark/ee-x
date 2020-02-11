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

#import "PurchasableVirtualItem.h"

@class VirtualCurrency;

/**
 Every game has its virtual currencies. Here you represent a pack of a specific 
 `VirtualCurrency`. Real Game Example: If virtual currency in your game is a 
 'Coin', you will sell packs of 'Coins' (e.g. "10 Coins Set" or "Super Saver 
 Pack").
 
 NOTE: In case you want this item to be available for purchase in the App Store 
 with real money, you'll need to define the item in iTunes Connect. 
 ( https://itunesconnect.apple.com )
 
 Inheritance: VirtualCurrencyPack > PurchasableVirtualItem > VirtualItem
 */
@interface VirtualCurrencyPack : PurchasableVirtualItem{
    int     currencyAmount;
    NSString* currencyItemId;
}

@property int     currencyAmount;
@property (retain, nonatomic) NSString* currencyItemId;

/** 
 Constructor
 
 @param oName see parent
 @param oDescription see parent
 @param oItemId see parent
 @param oCurrencyAmount the amount of currency in the pack.
 @param oCurrencyItemId the itemId of the currency associated with this pack.
 @param oPurchaseType see parent
 */
- (id)initWithName:(NSString*)oName andDescription:(NSString*)oDescription
     andItemId:(NSString*)oItemId andCurrencyAmount:(int)oCurrencyAmount andCurrency:(NSString*)oCurrencyItemId
     andPurchaseType:(PurchaseType*)oPurchaseType;

/** 
 Constructor
 
 @param see parent.
 */
- (id)initWithDictionary:(NSDictionary*)dict;

/**
 see parent.
 
 @return see parent.
 */
- (NSDictionary*)toDictionary;

@end
