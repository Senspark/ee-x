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

#import "Soomla.h"

#define SOOMLA_CORE_VERSION @"1.2.0"

@implementation Soomla

NSString* SOOMLA_SECRET = @"SOOMLA_SEC";

+ (void)initializeWithSecret:(NSString *)secret {
    SOOMLA_SECRET = secret;
}

+ (NSString*)getVersion {
    return SOOMLA_CORE_VERSION;
}

@end
