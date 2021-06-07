//
//  EEIronSourceBridge.h
//  Pods
//
//  Created by eps on 9/16/20.
//

#if __has_include("ee/IronSource.h")
// Build dynamic framework.
#import "ee/IronSource.h"
#else
#import <IronSource/IronSource.h>
#endif
