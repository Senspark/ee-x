//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/twitter/EETwitter.h"

#import <ReplayKit/ReplayKit.h>
#import <TwitterKit/TWTRKit.h>
#import <UIKit/UIKit.h>

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EETwitter () <TWTRComposerViewControllerDelegate> {
    EEMessageBridge* bridge_;
}

@end

@implementation EETwitter

// clang-format off
static NSString* const k__initialize       = @"twitter_initialize";
static NSString* const k__shareContent     = @"twitter_shareContent";
static NSString* const k__shareScreenShot  = @"twitter_shareScreenShot";
static NSString* const k__onSuccess        = @"twitter_onSuccess";
static NSString* const k__onFailure        = @"twitter_onFailure";
static NSString* const k__onCancel         = @"twitter_onCancel";
static NSString* const k__key              = @"twitter_key";
static NSString* const k__secret           = @"twitter_secret";
static NSString* const k__text             = @"twitter_text";
static NSString* const k__image            = @"twitter_image";
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
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* key = dict[k__key];
                        NSString* secret = dict[k__secret];
                        [self initialize:key secret:secret];
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
    [bridge_ deregisterHandler:k__shareScreenShot];
    [bridge_ deregisterHandler:k__shareContent];
}

- (void)initialize:(NSString*)key secret:(NSString*)secret {
    [[Twitter sharedInstance] startWithConsumerKey:key consumerSecret:secret];
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
                    NSLog(@"%s:%@", __PRETTY_FUNCTION__, @"Tweet not login");
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
                    NSLog(@"%s:%@", __PRETTY_FUNCTION__, @"Tweet not login");
                    [bridge_ callCpp:k__onFailure message:@""];
                }
            }];
    }
}

- (void)showTwitterShare:(NSString*)text image:(NSString*)image {
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    UIViewController* vc = [window rootViewController];

    UIImage* uiimage = nil;
    if (image != nil) {
        uiimage = [UIImage imageNamed:image];
    }

    TWTRComposerViewController* composer =
        [TWTRComposerViewController emptyComposer];
    [composer initWithInitialText:text image:uiimage videoURL:nil];
    composer.delegate = self;
    [vc presentViewController:composer animated:YES completion:nil];
}

- (void)composerDidCancel:(TWTRComposerViewController*)controller {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onCancel message:@""];
}

- (void)composerDidSucceed:(TWTRComposerViewController*)controller
                 withTweet:(TWTRTweet*)tweet {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onSuccess message:@""];
}

- (void)composerDidFail:(TWTRComposerViewController*)controller
              withError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onFailure message:@""];
}

- (BOOL)application:(UIApplication*)app
            openURL:(nonnull NSURL*)url
            options:(nonnull NSDictionary<NSString*, id>*)options {
    return [Twitter.sharedInstance application:app openURL:url options:options];
}

@end
