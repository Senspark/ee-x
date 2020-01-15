//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/recorder/EERecorder.h"

#import <ReplayKit/ReplayKit.h>

#import <ee/core/EEMessageBridge.h>
#import <ee/core/internal/EEDictionaryUtils.h>
#import <ee/core/internal/EEUtils.h>
#import <ee/core/internal/EEJsonUtils.h>

@interface EERecorder () <RPScreenRecorderDelegate,
                          RPPreviewViewControllerDelegate> {
    EEMessageBridge* bridge_;
    RPScreenRecorder* recorder_;
}

@end

@implementation EERecorder

// clang-format off
NSString* const k__startRecording           = @"Recorder_startRecording";
NSString* const k__stopRecording            = @"Recorder_stopRecording";
NSString* const k__cancelRecording          = @"Recorder_cancelRecording";
NSString* const k__getRecordingUrl          = @"Recorder_getRecordingUrl";
NSString* const k__checkRecordingPermission = @"Recorder_checkRecordingPermission";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    recorder_ = [RPScreenRecorder sharedRecorder];
    [recorder_ setDelegate:self];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [recorder_ setDelegate:nil];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__startRecording
                    callback:^(NSString* message) {
                        [self startRecording];
                        return @"";
                    }];

    [bridge_ registerHandler:k__stopRecording
                    callback:^(NSString* message) {
                        [self stopRecording];
                        return @"";
                    }];

    [bridge_ registerHandler:k__cancelRecording
                    callback:^(NSString* message) {
                        [self cancelRecording];
                        return @"";
                    }];

    [bridge_ registerHandler:k__getRecordingUrl
                    callback:^(NSString* message) {
                        return [self getRecordingUrl];
                    }];

    [bridge_ registerHandler:k__checkRecordingPermission
                    callback:^(NSString* message) {
                        return
                            [EEUtils toString:[self checkRecordingPermission]];
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__startRecording];
    [bridge_ deregisterHandler:k__stopRecording];
    [bridge_ deregisterHandler:k__cancelRecording];
    [bridge_ deregisterHandler:k__getRecordingUrl];
    [bridge_ deregisterHandler:k__checkRecordingPermission];
}

- (void)startRecording {
    [recorder_
        startRecordingWithMicrophoneEnabled:YES
                                    handler:^(NSError* error) {
                                        if (error != nil) {
                                            NSLog(@"startRecording: %@",
                                                  [error localizedDescription]);
                                        }
                                    }];
}

- (void)stopRecording {
    [recorder_ stopRecordingWithHandler:^(
                   RPPreviewViewController* previewViewController,
                   NSError* error) {
        if (error != nil) {
            NSLog(@"stopRecording: %@", [error localizedDescription]);
        }

        if (previewViewController != nil) {
            [previewViewController setPreviewControllerDelegate:self];
            [previewViewController
                setModalPresentationStyle:UIModalPresentationFullScreen];

            UIViewController* rootView = [EEUtils getCurrentRootViewController];
            [rootView presentViewController:previewViewController
                                   animated:YES
                                 completion:nil];
        }
    }];
}

- (void)cancelRecording {
    [recorder_
        stopRecordingWithHandler:^(
            RPPreviewViewController* previewViewController, NSError* error) {
            if (error != nil) {
                NSLog(@"cancelRecording: %@", [error localizedDescription]);
            }
        }];
}

- (BOOL)checkRecordingPermission {
    return [recorder_ isAvailable];
}

- (NSString*)getScreenRecordingUrl {
    return @"";
}

- (void)previewControllerDidFinish:(RPPreviewViewController*)previewController {
    [previewController dismissViewControllerAnimated:YES completion:nil];
}

@end
