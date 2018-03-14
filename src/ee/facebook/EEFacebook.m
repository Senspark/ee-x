//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/facebook/EEFacebook.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

@interface EEFacebook () <FBSDKSharingDelegate>
@end

@implementation EEFacebook

// clang-format off
NSString* const k__isLoggedIn        = @"Facebook_isLoggedIn";
NSString* const k__logIn             = @"Facebook_logIn";
NSString* const k__logOut            = @"Facebook_logOut";
NSString* const k__getAccessToken    = @"Facebook_getAccessToken";
NSString* const k__getUserId         = @"Facebook_getUserId";
NSString* const k__onLoginResult     = @"Facebook_onLoginResult";
NSString* const k__onProfileChanged  = @"Facebook_onProfileChanged";
NSString* const k__shareLinkContent  = @"Facebook_shareLinkContent";
NSString* const k__sharePhotoContent = @"Facebook_sharePhotoContent";
NSString* const k__shareVideoContent = @"Facebook_shareVideoContent";
NSString* const k__onShareResult     = @"Facebook_onShareResult";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];

    [[NSNotificationCenter defaultCenter]
        addObserverForName:FBSDKAccessTokenDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification* notification) {
                    [self accessTokenDidChange:notification];
                }];

    [[NSNotificationCenter defaultCenter]
        addObserverForName:FBSDKProfileDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification* notification) {
                    [self profileDidChange:notification];
                }];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__isLoggedIn
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self isLoggedIn]];
                   }];

    [bridge registerHandler:k__logIn
                   callback:^NSString* _Nonnull(NSString* _Nonnull message) {
                       NSArray* permissions =
                           [EEJsonUtils convertStringToArray:message];
                       [self logIn:permissions];
                       return @"";
                   }];

    [bridge registerHandler:k__logOut
                   callback:^NSString* _Nonnull(NSString* _Nonnull message) {
                       [self logOut];
                       return @"";
                   }];

    [bridge registerHandler:k__getAccessToken
                   callback:^(NSString* _Nonnull message) {
                       return [self getAccessToken];
                   }];

    [bridge registerHandler:k__getUserId
                   callback:^(NSString* _Nonnull message) {
                       return [self getUserId];
                   }];

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

    [bridge deregisterHandler:k__isLoggedIn];
    [bridge deregisterHandler:k__logIn];
    [bridge deregisterHandler:k__logOut];
    [bridge deregisterHandler:k__getAccessToken];
    [bridge deregisterHandler:k__getUserId];
    [bridge deregisterHandler:k__shareLinkContent];
    [bridge deregisterHandler:k__sharePhotoContent];
    [bridge deregisterHandler:k__shareVideoContent];
}

- (void)accessTokenDidChange:(NSNotification*)notification {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [notification userInfo]);
}

- (void)profileDidChange:(NSNotification*)notification {
    FBSDKProfile* profile = [FBSDKProfile currentProfile];
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, profile);
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    if (profile != nil) {
        [dict setObject:[profile userID] forKey:@"userId"];
        [dict setObject:[profile firstName] forKey:@"firstName"];
        [dict setObject:[profile middleName] ?: @"" forKey:@"middleName"];
        [dict setObject:[profile lastName] forKey:@"lastName"];
        [dict setObject:[profile name] forKey:@"name"];
        [dict setObject:[[profile
                            imageURLForPictureMode:FBSDKProfilePictureModeSquare
                                              size:CGSizeMake(128, 128)]
                            absoluteString]
                 forKey:@"picture"];
    }

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onProfileChanged
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (BOOL)isLoggedIn {
    return [FBSDKAccessToken currentAccessToken] != nil;
}

- (void)logIn:(NSArray* _Nonnull)permissions {
    FBSDKLoginManagerRequestTokenHandler handler = ^(
        FBSDKLoginManagerLoginResult* result, NSError* error) {
        EEMessageBridge* bridge = [EEMessageBridge getInstance];
        if (error != nil) {
            [bridge callCpp:k__onLoginResult message:[EEUtils toString:NO]];
        } else if ([result isCancelled]) {
            [bridge callCpp:k__onLoginResult message:[EEUtils toString:NO]];
        } else {
            [bridge callCpp:k__onLoginResult message:[EEUtils toString:YES]];
        }
    };

    FBSDKLoginManager* loginManager =
        [[[FBSDKLoginManager alloc] init] autorelease];
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [loginManager logInWithReadPermissions:permissions
                        fromViewController:rootView
                                   handler:handler];
}

- (void)logOut {
    FBSDKLoginManager* loginManager =
        [[[FBSDKLoginManager alloc] init] autorelease];
    [loginManager logOut];
}

- (NSString* _Nonnull)getAccessToken {
    if (![self isLoggedIn]) {
        return @"";
    }
    return [[FBSDKAccessToken currentAccessToken] tokenString];
}

- (NSString* _Nonnull)getUserId {
    if (![self isLoggedIn]) {
        return @"";
    }
    return [[FBSDKAccessToken currentAccessToken] userID];
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
