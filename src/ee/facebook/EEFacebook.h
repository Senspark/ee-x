//
//  Created by Zinge on 7/4/16.
//
//

#import <Foundation/Foundation.h>

@interface EEFacebook : NSObject

- (BOOL)isLoggedIn;
- (void)logIn:(NSArray* _Nonnull)permissions;
- (void)logOut;
- (NSString* _Nonnull)getAccessToken;
- (NSString* _Nonnull)getUserId;

- (void)shareLinkContent:(NSString* _Nonnull)url;
- (void)sharePhotoContent:(NSString* _Nonnull)url;
- (void)shareVideoContent:(NSString* _Nonnull)url;

@end
