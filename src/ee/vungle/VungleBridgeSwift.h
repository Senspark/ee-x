//
//  VungleBridgeSwift.h
//  Pods
//
//  Created by eps on 6/25/20.
//

#ifdef __OBJC__
/// https://support.vungle.com/hc/en-us/articles/360002925791-Integrate-Vungle-SDK-for-iOS
/// Step 3: Swift Only: Create a Bridging Header File.
#if EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISVungleAdapter/VungleSDK.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <VungleSDK/VungleSDK.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION
#endif // __OBJC__
