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

#import <Foundation/Foundation.h>

/**
 This exception is thrown when a user tries to make a purchase and does not 
 have enough funds.
 
 Real Game Example:
  Example Inventory: { currency_coin: 100, green_hat: 3, blue_hat: 5 }.
  Say a blue_hat costs 200 currency_coin.
  Suppose that your user wants to buy a blue_hat.
  He/she will probably call `StoreInventory.buy("blue_hat")`.
  `InsufficientFundsException` will be thrown with "blue_hat" as the `itemId`.
  You can just catch this exception in order to notify the user that he/she 
  doesn't have enough coins to buy a blue_hat.
 */
@interface InsufficientFundsException : NSException{
    NSString* itemId;
}

@property (nonatomic, retain) NSString* itemId;

/**
 Constructor
 
 @param itemId The id of the item that caused the exception.
 */
- (id)initWithItemId:(NSString*)itemId;

@end


