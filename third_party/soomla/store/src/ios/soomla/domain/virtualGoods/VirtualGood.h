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

/**
 This is an abstract representation of the application's virtual goods.
 Your game's virtual economy revolves around virtual goods. This class defines 
 the abstract and most common virtual goods while the descendants of this class
 define specific definitions of virtual goods.
 
 Inheritance: VirtualGood > PurchasableVirtualItem > VirtualItem
 */
@interface VirtualGood : PurchasableVirtualItem{
}

/** 
 Constructor
 
 @param oName see parent.
 @param oDescription see parent.
 @param oItemId see parent.
 @param oPurchaseType see parent.
 */
- (id)initWithName:(NSString*)oName andDescription:(NSString*)oDescription
         andItemId:(NSString*)oItemId andPurchaseType:(PurchaseType*)oPurchaseType;

@end
