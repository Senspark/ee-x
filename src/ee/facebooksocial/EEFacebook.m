//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/facebooksocial/EEFacebook.h"
#import <ReplayKit/ReplayKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

@interface EEFacebook() <FBSDKSharingDelegate, RPScreenRecorderDelegate, RPPreviewViewControllerDelegate>

@end

@implementation EEFacebook

// clang-format off
NSString* const k__facebook_share_link_content              = @"__facebook_share_link_content";
NSString* const k__facebook_share_photo_content             = @"__facebook_share_photo_content";
NSString* const k__onResult                                 = @"__facebook_share_onResult";

NSString* const k__facebook_share_video_content             = @"__facebook_share_video_content";
NSString* const k__facebook_get_video_record_url            = @"__facebook_get_video_record_url";
NSString* const k__facebook_record_screen                   = @"__facebook_record_screen";
NSString* const k__facebook_stop_record_screen              = @"__facebook_stop_record_screen";

NSString* const k__facebook_cancel_record_screen            = @"__facebook_cancel_record_screen";
NSString* const k__facebook_check_permission_record         = @"__facebook_check_permission_record";

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
    
    [bridge registerHandler:^(NSString* msg) {
        NSString* name = msg;
        [self shareVideoContent:name];
        return @"";
    } tag:k__facebook_share_video_content];
    
    [bridge registerHandler:^(NSString* msg) {
        [self recordScreen];
        return @"";
    } tag:k__facebook_record_screen];
    
    [bridge registerHandler:^(NSString* msg) {
        [self stopRecordScreen];
        return @"";
    } tag:k__facebook_stop_record_screen];
    
    [bridge registerHandler:^(NSString* msg) {
        return [EEUtils toString:[self checkPermissionRecord]];
    } tag:k__facebook_check_permission_record];
    
    [bridge registerHandler:^(NSString* msg) {
        return [self getVideoRecordUrl];
    } tag:k__facebook_get_video_record_url];
    
    [bridge registerHandler:^(NSString* msg) {
        [self stopRecordScreen];
        [self cancelRecordScreen];
        return @"";
    } tag:k__facebook_cancel_record_screen];
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
    
    [FBSDKShareDialog showFromViewController:rootView withContent:content delegate:self];
}

- (void)shareVideoContent:(NSString*)filename {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    
    NSURL *videoURL = [NSURL URLWithString:filename];
    
    FBSDKShareVideo *video = [[FBSDKShareVideo alloc] init];
    video.videoURL = videoURL;
    FBSDKShareVideoContent *content = [[FBSDKShareVideoContent alloc] init];
    content.video = video;
    
    [FBSDKShareDialog showFromViewController:rootView withContent:content delegate:self];
}

- (void) recordScreen
{
    RPScreenRecorder *sharedRecorder = RPScreenRecorder.sharedRecorder;
    sharedRecorder.delegate = self;
    [sharedRecorder startRecordingWithMicrophoneEnabled:YES handler:^(NSError *error) {
        if (error) {
            NSLog(@"startScreenRecording: %@", error.localizedDescription);
        }
    }];
}

- (void) stopRecordScreen
{
    RPScreenRecorder *sharedRecorder = RPScreenRecorder.sharedRecorder;
    [sharedRecorder stopRecordingWithHandler:^(RPPreviewViewController *previewViewController, NSError *error) {
        if (error) {
            NSLog(@"stopScreenRecording: %@", error.localizedDescription);
        }
        
        if (previewViewController) {
            previewViewController.previewControllerDelegate = self;
            
            previewViewController.modalPresentationStyle = UIModalPresentationFullScreen;
            UIViewController* rootView = [EEUtils getCurrentRootViewController];
            [rootView presentViewController:previewViewController animated:YES completion:nil];
        }
    }];
}

-(void) cancelRecordScreen
{
    
}

- (BOOL) checkPermissionRecord
{
    return YES;
}

-(NSString*) getVideoRecordUrl
{
    return @"";
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

#pragma mark - RPPreviewViewControllerDelegate
- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController {
    [previewController dismissViewControllerAnimated:YES completion:nil];
}
@end
