/// Used in ee-Swift.h
#if __has_include(<IronSource/IronSource.h>)
#import <IronSource/ISInterstitialDelegate.h>
#import <IronSource/ISRewardedVideoDelegate.h>
#endif
#if EE_X_USE_IRON_SOURCE_MEDIATION
#if __has_include(<ISVungleAdapter/VungleSDK.h>)
#import <ISVungleAdapter/VungleSDK.h>
#endif
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#if __has_include(<VungleSDK/VungleSDK.h>)
#import <VungleSDK/VungleSDK.h>
#endif
#endif // EE_X_USE_IRON_SOURCE_MEDIATION
