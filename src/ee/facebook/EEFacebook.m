//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/facebook/EEFacebook.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

#import <ee-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

#import "ee/facebook/private/EEFacebookRequestDelegate.h"
#import "ee/facebook/private/EEFacebookShareDelegate.h"

@implementation EEFacebook {
    id<EEIMessageBridge> bridge_;
}

// clang-format off
NSString* const k__registerNotifications = @"Facebook_registerNotifications";
NSString* const k__isLoggedIn            = @"Facebook_isLoggedIn";
NSString* const k__logIn                 = @"Facebook_logIn";
NSString* const k__logOut                = @"Facebook_logOut";
NSString* const k__getAccessToken        = @"Facebook_getAccessToken";
NSString* const k__onProfileChanged      = @"Facebook_onProfileChanged";
NSString* const k__graphRequest          = @"Facebook_graphRequest";
NSString* const k__sendRequest           = @"Facebook_sendRequest";
NSString* const k__shareLinkContent      = @"Facebook_shareLinkContent";
NSString* const k__sharePhotoContent     = @"Facebook_sharePhotoContent";
NSString* const k__shareVideoContent     = @"Facebook_shareVideoContent";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];
    [FBSDKSettings setAutoLogAppEventsEnabled:YES];
    [FBSDKSettings setAdvertiserIDCollectionEnabled:YES];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerNotifications {
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
}

- (void)registerHandlers {
    [bridge_ registerHandler:
        k__registerNotifications:^(NSString* message) {
            [self registerNotifications];
            return @"";
        }];

    [bridge_ registerHandler:
               k__isLoggedIn:^(NSString* message) {
                   return [EEUtils toString:[self isLoggedIn]];
               }];

    [bridge_ registerHandler:
                    k__logIn:^(NSString* _Nonnull message) {
                        return [self logIn:message];
                    }];

    [bridge_ registerHandler:
                   k__logOut:^(NSString* _Nonnull message) {
                       [self logOut];
                       return @"";
                   }];

    [bridge_ registerHandler:
           k__getAccessToken:^(NSString* _Nonnull message) {
               FBSDKAccessToken* token = [self getAccessToken];
               return [self convertAccessTokenToString:token];
           }];

    [bridge_ registerHandler:
             k__graphRequest:^(NSString* message) {
                 return [self graphRequest:message];
             }];

    [bridge_ registerHandler:
              k__sendRequest:^(NSString* message) {
                  return [self sendRequest:message];
              }];

    [bridge_ registerHandler:
         k__shareLinkContent:^(NSString* message) {
             return [self shareLinkContent:message];
         }];

    [bridge_ registerHandler:
        k__sharePhotoContent:^(NSString* message) {
            return [self sharePhotoContent:message];
        }];

    [bridge_ registerHandler:
        k__shareVideoContent:^(NSString* message) {
            return [self shareVideoContent:message];
        }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__registerNotifications];
    [bridge_ deregisterHandler:k__isLoggedIn];
    [bridge_ deregisterHandler:k__logIn];
    [bridge_ deregisterHandler:k__logOut];
    [bridge_ deregisterHandler:k__getAccessToken];
    [bridge_ deregisterHandler:k__graphRequest];
    [bridge_ deregisterHandler:k__sendRequest];
    [bridge_ deregisterHandler:k__shareLinkContent];
    [bridge_ deregisterHandler:k__sharePhotoContent];
    [bridge_ deregisterHandler:k__shareVideoContent];
}

- (BOOL)application:(UIApplication*)application
               open:(NSURL*)url
            options:(NSDictionary<UIApplicationOpenURLOptionsKey, id>*)options {
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                          options:options];
}

- (BOOL)application:(UIApplication*)application
                 open:(NSURL*)url
    sourceApplication:(NSString*)sourceApplication
           annotation:(id)annotation {
    return
        [[FBSDKApplicationDelegate sharedInstance] application:application
                                                       openURL:url
                                             sourceApplication:sourceApplication
                                                    annotation:annotation];
}

- (NSString* _Nonnull)convertAccessTokenToString:(FBSDKAccessToken*)token {
    if (token == nil) {
        return @"";
    }
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setObject:[token tokenString] forKey:@"token"];
    [dict setObject:[token appID] forKey:@"applicationId"];
    [dict setObject:[token userID] forKey:@"userId"];
    return [EEJsonUtils convertDictionaryToString:dict];
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

    [bridge_ callCpp:k__onProfileChanged //
                    :[EEJsonUtils convertDictionaryToString:dict]];
}

- (BOOL)isLoggedIn {
    return [FBSDKAccessToken currentAccessToken] != nil;
}

- (NSString* _Nonnull)k__onLoginSuccess:(int)tag {
    return [@"FacebookLoginDelegate_onSuccess_"
        stringByAppendingString:[@(tag) stringValue]];
}

- (NSString* _Nonnull)k__onLoginFailure:(int)tag {
    return [@"FacebookLoginDelegate_onFailure_"
        stringByAppendingString:[@(tag) stringValue]];
}

- (NSString* _Nonnull)k__onLoginCancel:(int)tag {
    return [@"FacebookLoginDelegate_onCancel_"
        stringByAppendingString:[@(tag) stringValue]];
}

- (NSString* _Nonnull)logIn:(NSString* _Nonnull)message {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
    NSArray* permissions = [dict objectForKey:@"permissions"];
    int tag = [[dict objectForKey:@"tag"] intValue];

    if ([FBSDKAccessToken currentAccessToken]) {
        // do something when already logged in
        return @"";
    }

    FBSDKLoginManagerLoginResultBlock block = ^(
        FBSDKLoginManagerLoginResult* _Nullable result,
        NSError* _Nullable error) {
        if (error != nil) {
            [bridge_ callCpp:[self k__onLoginFailure:tag] //
                            :[error localizedDescription]];
        } else if ([result isCancelled]) {
            [bridge_ callCpp:[self k__onLoginCancel:tag]];
        } else {
            [bridge_ callCpp:[self k__onLoginSuccess:tag] //
                            :[self convertAccessTokenToString:[result token]]];
        }
    };

    [self logIn:permissions handler:block];
    return @"";
}

- (void)logIn:(NSArray* _Nonnull)permissions
      handler:(FBSDKLoginManagerLoginResultBlock _Nonnull)handler {
    FBSDKLoginManager* loginManager =
        [[[FBSDKLoginManager alloc] init] autorelease];
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [loginManager logInWithPermissions:permissions
                    fromViewController:rootView
                               handler:handler];
}

- (void)logOut {
    FBSDKLoginManager* loginManager =
        [[[FBSDKLoginManager alloc] init] autorelease];
    [loginManager logOut];
}

- (FBSDKAccessToken* _Nullable)getAccessToken {
    return [FBSDKAccessToken currentAccessToken];
}

- (NSString* _Nonnull)k__onGraphSuccess:(int)tag {
    return [@"FacebookGraphDelegate_onSuccess_"
        stringByAppendingString:[@(tag) stringValue]];
}

- (NSString* _Nonnull)k__onGraphFailure:(int)tag {
    return [@"FacebookGraphDelegate_onFailure_"
        stringByAppendingString:[@(tag) stringValue]];
}

- (NSString* _Nonnull)graphRequest:(NSString* _Nonnull)message {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
    int tag = [[dict objectForKey:@"tag"] intValue];
    NSString* path = [dict objectForKey:@"path"];
    NSDictionary* parameters = [dict objectForKey:@"parameters"];

    FBSDKGraphRequestBlock handler =
        ^(FBSDKGraphRequestConnection* connection, id result, NSError* error) {
            if (error != nil) {
                [bridge_ callCpp:[self k__onGraphFailure:tag] //
                                :[error localizedDescription]];
            } else {
                [bridge_ callCpp:[self k__onGraphSuccess:tag] //
                                :[EEJsonUtils convertObjectToString:result]];
            }
        };
    [self graphRequest:path parameters:parameters handler:handler];
    return @"";
}

- (void)graphRequest:(NSString* _Nonnull)path
          parameters:(NSDictionary* _Nullable)parameters
             handler:(FBSDKGraphRequestBlock _Nonnull)handler {
    FBSDKGraphRequest* request =
        [[[FBSDKGraphRequest alloc] initWithGraphPath:path
                                           parameters:parameters] autorelease];

    [request startWithCompletionHandler:handler];
}

- (NSString* _Nonnull)sendRequest:(NSString* _Nonnull)message_ {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message_];
    int tag = [[dict objectForKey:@"tag"] intValue];
    int actionType = [[dict objectForKey:@"actionType"] intValue];
    int filter = [[dict objectForKey:@"filter"] intValue];
    NSString* title = [dict objectForKey:@"title"];
    NSArray* recipients = [dict objectForKey:@"recipients"];
    NSString* objectId = [dict objectForKey:@"objectId"];
    NSString* data = [dict objectForKey:@"data"];
    NSString* message = [dict objectForKey:@"message"];
    [self sendRequest:(FBSDKGameRequestActionType)actionType
               filter:(FBSDKGameRequestFilter)filter
                title:title
           recipients:recipients
             objectId:objectId
                 data:data
              message:message
             delegate:[EEFacebookRequestDelegate delegateWithBridge:bridge_
                                                                tag:tag]];
    return @"";
}

- (void)sendRequest:(FBSDKGameRequestActionType)actionType
             filter:(FBSDKGameRequestFilter)filter
              title:(NSString* _Nullable)title
         recipients:(NSArray* _Nullable)recipients
           objectId:(NSString* _Nullable)objectId
               data:(NSString* _Nullable)data
            message:(NSString* _Nonnull)message
           delegate:(id<FBSDKGameRequestDialogDelegate> _Nonnull)delegate {
    FBSDKGameRequestContent* content =
        [[[FBSDKGameRequestContent alloc] init] autorelease];
    [content setActionType:actionType];
    [content setFilters:filter];
    [content setObjectID:objectId];
    [content setTitle:title];
    [content setRecipients:recipients];
    [content setData:data];
    [content setMessage:message];
    [FBSDKGameRequestDialog showWithContent:content delegate:[delegate retain]];
}

- (NSString* _Nonnull)shareLinkContent:(NSString* _Nonnull)message {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
    int tag = [[dict objectForKey:@"tag"] intValue];
    NSString* url = [dict objectForKey:@"url"];
    [self shareLinkContent:url
                  delegate:[EEFacebookShareDelegate delegateWithBridge:bridge_
                                                                   tag:tag]];
    return @"";
}

- (void)shareLinkContent:(NSString* _Nonnull)url
                delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate {
    FBSDKShareLinkContent* content =
        [[[FBSDKShareLinkContent alloc] init] autorelease];

    NSString* escapedUrl =
        [url stringByAddingPercentEncodingWithAllowedCharacters:
                 [NSCharacterSet URLQueryAllowedCharacterSet]];
    [content setContentURL:[NSURL URLWithString:escapedUrl]];

    [self shareContent:content delegate:delegate];
}

- (NSString* _Nonnull)sharePhotoContent:(NSString* _Nonnull)message {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
    int tag = [[dict objectForKey:@"tag"] intValue];
    NSString* url = [dict objectForKey:@"url"];
    [self sharePhotoContent:url
                   delegate:[EEFacebookShareDelegate delegateWithBridge:bridge_
                                                                    tag:tag]];
    return @"";
}

- (void)sharePhotoContent:(NSString* _Nonnull)url
                 delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate {
    FBSDKSharePhoto* photo = [[[FBSDKSharePhoto alloc] init] autorelease];
    [photo setImage:[UIImage imageNamed:url]];
    [photo setUserGenerated:YES];

    FBSDKSharePhotoContent* content =
        [[[FBSDKSharePhotoContent alloc] init] autorelease];
    [content setPhotos:@[photo]];

    [self shareContent:content delegate:delegate];
}

- (NSString* _Nonnull)shareVideoContent:(NSString* _Nonnull)message {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message];
    int tag = [[dict objectForKey:@"tag"] intValue];
    NSString* url = [dict objectForKey:@"url"];
    [self shareVideoContent:url
                   delegate:[EEFacebookShareDelegate delegateWithBridge:bridge_
                                                                    tag:tag]];
    return @"";
}

- (void)shareVideoContent:(NSString* _Nonnull)url
                 delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate {
    FBSDKShareVideo* video = [[[FBSDKShareVideo alloc] init] autorelease];
    [video setVideoURL:[NSURL URLWithString:url]];

    FBSDKShareVideoContent* content =
        [[[FBSDKShareVideoContent alloc] init] autorelease];
    [content setVideo:video];

    [self shareContent:content delegate:delegate];
}

- (void)shareContent:(id<FBSDKSharingContent> _Nonnull)content
            delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [FBSDKShareDialog showFromViewController:rootView
                                 withContent:content
                                    delegate:[delegate retain]];
}

@end
