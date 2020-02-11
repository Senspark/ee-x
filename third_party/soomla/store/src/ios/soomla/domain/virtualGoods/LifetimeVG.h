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

#import "VirtualGood.h"

/**
 A `LifetimeVG` is a virtual good that is bought only once and kept forever.
 
 The `LifetimeVG`'s characteristics are:
  1. Can only be purchased once.
  2. Your users cannot have more than one of this item.
 
 Real Games Examples: 'No Ads', 'Double Coins'
 
 This `VirtualItem` is purchasable.
 In case you want this item to be available for purchase in the App Store, 
 you'll need to define the app store item in iTunes Connect. 
 ( https://itunesconnect.apple.com )
 
 Inheritance: LifeTimeVG > VirtualGood > PurchasableVirtualItem > VirtualItem
 */
@interface LifetimeVG : VirtualGood

@end
