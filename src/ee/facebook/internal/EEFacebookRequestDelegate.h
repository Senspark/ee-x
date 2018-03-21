//
//  EEFacebookRequestDelegate.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#import <FBSDKShareKit/FBSDKGameRequestDialog.h>

@protocol EEIMessageBridge;

@interface EEFacebookRequestDelegate : NSObject <FBSDKGameRequestDialogDelegate>

+ (instancetype)delegateWithBridge:(id<EEIMessageBridge>)bridge tag:(int)tag;

@end
