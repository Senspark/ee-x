//
//  EECampaignReceiver.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EECampaignReceiver : NSObject

+ (BOOL)application:(UIApplication*)app
            openURL:(nonnull NSURL*)url
            options:(nonnull NSDictionary<NSString*, id>*)options;

@end
