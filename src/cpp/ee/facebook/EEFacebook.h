//
//  Created by Zinge on 7/4/16.
//
//

#import <FBSDKCoreKit/FBSDKGraphRequestConnection.h>
#import <FBSDKLoginKit/FBSDKLoginManager.h>
#import <FBSDKShareKit/FBSDKGameRequestContent.h>

@protocol EEIPlugin;

@protocol FBSDKGameRequestDialogDelegate;
@protocol FBSDKSharingDelegate;
@class FBSDKAccessToken;

@interface EEFacebook : NSObject <EEIPlugin>

- (BOOL)isLoggedIn;
- (void)logIn:(NSArray* _Nonnull)permissions
      handler:(FBSDKLoginManagerLoginResultBlock _Nonnull)handler;
- (void)logOut;

- (FBSDKAccessToken* _Nullable)getAccessToken;

- (void)graphRequest:(NSString* _Nonnull)path
          parameters:(NSDictionary* _Nullable)parameters
             handler:(FBSDKGraphRequestBlock _Nonnull)handler;

- (void)sendRequest:(FBSDKGameRequestActionType)actionType
             filter:(FBSDKGameRequestFilter)filter
              title:(NSString* _Nullable)title
         recipients:(NSArray* _Nullable)recipients
           objectId:(NSString* _Nullable)objectId
               data:(NSString* _Nullable)data
            message:(NSString* _Nonnull)message
           delegate:(id<FBSDKGameRequestDialogDelegate> _Nonnull)delegate;

- (void)shareLinkContent:(NSString* _Nonnull)url
                delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate;

- (void)sharePhotoContent:(NSString* _Nonnull)url
                 delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate;

- (void)shareVideoContent:(NSString* _Nonnull)url
                 delegate:(id<FBSDKSharingDelegate> _Nonnull)delegate;

@end
