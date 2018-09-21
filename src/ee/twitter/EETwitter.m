//
//  Created by Zinge on 7/4/16.
//
//

#import <TwitterKit/TWTRKit.h>
#import <UIKit/UIKit.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/twitter/EETwitter.h"

#import <ReplayKit/ReplayKit.h>

@interface EETwitter () {
    EEMessageBridge* bridge_;
}

@end

@implementation EETwitter

// clang-format off
NSString* const k__initialize       = @"Twitter_initialize";
NSString* const k__shareContent     = @"Twitter_shareContent";
NSString* const k__shareScreenShot  = @"Twitter_shareScreenShot";
NSString* const k__onSuccess        = @"Twitter_onSuccess";
NSString* const k__onFailure        = @"Twitter_onFailure";
NSString* const k__onCancel         = @"Twitter_onCancel";
// clang-format on

// clang-format off
static NSString* const k__text   = @"twitter_text";
static NSString* const k__image  = @"twitter_image";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        [self initialize];
                        return @"";
                    }];

    [bridge_ registerHandler:k__shareScreenShot
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* text = dict[k__text];
                        NSString* image = dict[k__image];
                        [self shareScreenShot:text image:image];
                        return @"";
                    }];

    [bridge_ registerHandler:k__shareContent
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* text = dict[k__text];
                        [self shareContent:text];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__shareContent];
}

- (void)initialize {
    [[Twitter sharedInstance]
        startWithConsumerKey:@"WgRdd92MiYn1L4JiskaGfl8wK"
              consumerSecret:
                  @"FYlWKjtQXDedTRaiuyWF9nl6wIWu243QaI0BMIitrW2GMna9Aq"];
}

- (void)shareContent:(NSString*)text {
    // Check if current session has users logged in
    if ([[Twitter sharedInstance].sessionStore hasLoggedInUsers]) {
        [self showTwitterShare:text image:nil];
    } else {
        [[Twitter sharedInstance]
            logInWithCompletion:^(TWTRSession* session, NSError* error) {
                if (session) {
                    [self showTwitterShare:text image:nil];
                } else {
                    NSLog(@"Tweet not login");
                    [bridge_ callCpp:k__onFailure message:@""];
                }
            }];
    }
}

- (void)shareScreenShot:(NSString*)text image:(NSString*)image {
    // Check if current session has users logged in
    if ([[Twitter sharedInstance].sessionStore hasLoggedInUsers]) {
        [self showTwitterShare:text image:image];
    } else {
        [[Twitter sharedInstance]
            logInWithCompletion:^(TWTRSession* session, NSError* error) {
                if (session) {
                    [self showTwitterShare:text image:image];
                } else {
                    NSLog(@"Tweet not login");
                    [bridge_ callCpp:k__onFailure message:@""];
                }
            }];
    }
}

- (void)showTwitterShare:(NSString*)text image:(NSString*)image {
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    UIViewController* vc = [window rootViewController];

    TWTRComposer* composer = [[TWTRComposer alloc] init];
    [composer setText:text];

    if (image != nil) {
        [composer setImage:[UIImage imageNamed:image]];
    }

    // Called from a UIViewController
    [composer showFromViewController:vc
                          completion:^(TWTRComposerResult result) {
                              if (result == TWTRComposerResultCancelled) {
                                  NSLog(@"Tweet composition cancelled");
                                  [bridge_ callCpp:k__onCancel message:@""];
                              } else {
                                  NSLog(@"Sending Tweet!");
                                  [bridge_ callCpp:k__onSuccess message:@""];
                              }
                          }];
}

@end
