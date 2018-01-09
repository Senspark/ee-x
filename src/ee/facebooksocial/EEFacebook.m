//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/facebooksocial/EEFacebook.h"

#import <FBSDKShareKit/FBSDKShareKit.h>

@interface EEFacebook() <FBSDKSharingDelegate>

@end

@implementation EEFacebook

// clang-format off
NSString* const k__facebook_share_link_content              = @"__facebook_share_link_content";
NSString* const k__facebook_share_photo_content             = @"__facebook_share_photo_content";
NSString* const k__onResult                                 = @"__onResult";
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

    [bridge registerHandler:^(NSString* msg) {
        NSString* url = msg;
        [self shareLinkContent:url];
        return @"";
    } tag:k__facebook_share_link_content];
    
    [bridge registerHandler:^(NSString* msg) {
        NSString* name = msg;
        [self sharePhotoContent:name];
        return @"";
    } tag:k__facebook_share_photo_content];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__facebook_share_link_content];
    [bridge deregisterHandler:k__facebook_share_photo_content];
}

- (void)shareLinkContent:(NSString*)urlString {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];

    FBSDKShareLinkContent* content = [[FBSDKShareLinkContent alloc] init];
    content.contentURL = [NSURL URLWithString:urlString];

//    FBSDKShareDialog* dialog = [[FBSDKShareDialog alloc] init];
//    dialog.fromViewController = rootView;
//    dialog.shareContent = content;
//    dialog.mode = FBSDKShareDialogModeAutomatic;
//    [dialog show];
    
    [FBSDKShareDialog showFromViewController:rootView withContent:content delegate:self];
}

- (void)sharePhotoContent:(NSString*)filename {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    UIImage* image = [UIImage imageNamed:filename];

    FBSDKSharePhoto* photo = [[FBSDKSharePhoto alloc] init];
    photo.image = image;
    photo.userGenerated = YES;
    FBSDKSharePhotoContent* content = [[FBSDKSharePhotoContent alloc] init];
    content.photos = @[photo];

//    FBSDKShareDialog* dialog = [[FBSDKShareDialog alloc] init];
//    dialog.fromViewController = rootView;
//    dialog.shareContent = content;
//    dialog.mode = FBSDKShareDialogModeNative;
//    [dialog show];
    
    [FBSDKShareDialog showFromViewController:rootView withContent:content delegate:self];
}

#pragma mark - FBSDKSharingDelegate
- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results
{
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onResult message:@"true"];
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error
{
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onResult message:@"false"];
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer
{
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onResult message:@"false"];
}

@end
