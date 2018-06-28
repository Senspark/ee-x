//
//  EEIPlugin.h
//  ee_x
//
//  Created by eps on 6/28/18.
//

#import <Foundation/Foundation.h>

@class UIApplication;
@protocol UIUserActivityRestoring;

typedef void (^EERestorationHandler)(
    NSArray<id<UIUserActivityRestoring>>* _Nullable restorableObjects);

@protocol EEIPlugin <NSObject>

@optional

- (BOOL)application:(UIApplication* _Nonnull)application
              openURL:(NSURL* _Nonnull)url
    sourceApplication:(NSString* _Nonnull)sourceApplication
           annotation:(id _Nonnull)annotation;

- (BOOL)application:(UIApplication* _Nonnull)application
            openURL:(NSURL* _Nonnull)url
            options:(NSDictionary* _Nonnull)options;

- (BOOL)application:(UIApplication* _Nonnull)application
    continueUserActivity:(NSUserActivity* _Nonnull)userActivity
      restorationHandler:(EERestorationHandler _Nonnull)restorationHandler;

@end
