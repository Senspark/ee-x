//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/facebook/EEFacebook.h"

#import <FBSDKShareKit/FBSDKShareKit.h>

@interface EEFacebook () <FBSDKSharingDelegate>

@end

@implementation EEFacebook

// clang-format off
NSString* const k__shareLinkContent  = @"Facebook_shareLinkContent";
NSString* const k__sharePhotoContent = @"Facebook_sharePhotoContent";
NSString* const k__shareVideoContent = @"Facebook_shareVideoContent";
NSString* const k__onShareResult     = @"Facebook_shareOnResult";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__shareLinkContent
                   callback:^(NSString* message) {
                       NSString* url = message;
                       [self shareLinkContent:url];
                       return @"";
                   }];

    [bridge registerHandler:k__sharePhotoContent
                   callback:^(NSString* message) {
                       NSString* url = message;
                       [self sharePhotoContent:url];
                       return @"";
                   }];

    [bridge registerHandler:k__shareVideoContent
                   callback:^(NSString* message) {
                       NSString* url = message;
                       [self shareVideoContent:url];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__shareLinkContent];
    [bridge deregisterHandler:k__sharePhotoContent];
    [bridge deregisterHandler:k__shareVideoContent];
}

- (void)shareLinkContent:(NSString*)url {
    FBSDKShareLinkContent* content =
        [[[FBSDKShareLinkContent alloc] init] autorelease];
    [content setContentURL:[NSURL URLWithString:url]];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [FBSDKShareDialog showFromViewController:rootView
                                 withContent:content
                                    delegate:self];
}

- (void)sharePhotoContent:(NSString*)url {
    FBSDKSharePhoto* photo = [[[FBSDKSharePhoto alloc] init] autorelease];
    [photo setImage:[UIImage imageNamed:url]];
    [photo setUserGenerated:YES];

    FBSDKSharePhotoContent* content =
        [[[FBSDKSharePhotoContent alloc] init] autorelease];
    [content setPhotos:@[photo]];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [FBSDKShareDialog showFromViewController:rootView
                                 withContent:content
                                    delegate:self];
}

- (void)shareVideoContent:(NSString*)url {
    FBSDKShareVideo* video = [[[FBSDKShareVideo alloc] init] autorelease];
    [video setVideoURL:[NSURL URLWithString:url]];

    FBSDKShareVideoContent* content =
        [[[FBSDKShareVideoContent alloc] init] autorelease];
    [content setVideo:video];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [FBSDKShareDialog showFromViewController:rootView
                                 withContent:content
                                    delegate:self];
}

- (void)sharer:(id<FBSDKSharing>)sharer
    didCompleteWithResults:(NSDictionary*)results {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onShareResult message:[EEUtils toString:YES]];
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError*)error {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onShareResult message:[EEUtils toString:NO]];
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onShareResult message:[EEUtils toString:NO]];
}

@end
