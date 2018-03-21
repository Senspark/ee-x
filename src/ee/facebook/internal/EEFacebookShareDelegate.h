//
//  EEFacebookShareDelegate.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#import <FBSDKShareKit/FBSDKSharing.h>

@protocol EEIMessageBridge;

@interface EEFacebookShareDelegate : NSObject <FBSDKSharingDelegate>

+ (instancetype)delegateWithBridge:(id<EEIMessageBridge>)bridge tag:(int)tag;

@end
