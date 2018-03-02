//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/recorder/EERecorder.h"

#import <ReplayKit/ReplayKit.h>

@interface EERecorder () <RPScreenRecorderDelegate,
                          RPPreviewViewControllerDelegate>

@end

@implementation EERecorder

// clang-format off
NSString* const k__startScreenRecording     = @"Recorder_startScreenRecording";
NSString* const k__stopScreenRecording      = @"Recorder_stopScreenRecording";
NSString* const k__cancelScreenRecording    = @"Recorder_cancelScreenRecording";
NSString* const k__getScreenRecordingUrl    = @"Recorder_getScreenRecordingUrl";
NSString* const k__checkRecordingPermission = @"Recorder_checkRecordingPermission";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    [recorder setDelegate:self];

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];

    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    [recorder setDelegate:nil];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__startScreenRecording
                   callback:^(NSString* message) {
                       [self startScreenRecording];
                       return @"";
                   }];

    [bridge registerHandler:k__stopScreenRecording
                   callback:^(NSString* message) {
                       [self stopScreenRecording];
                       return @"";
                   }];

    [bridge registerHandler:k__cancelScreenRecording
                   callback:^(NSString* message) {
                       [self cancelScreenRecording];
                       return @"";
                   }];

    [bridge registerHandler:k__getScreenRecordingUrl
                   callback:^(NSString* message) {
                       return [self getScreenRecordingUrl];
                   }];

    [bridge registerHandler:k__checkRecordingPermission
                   callback:^(NSString* message) {
                       return
                           [EEUtils toString:[self checkRecordingPermission]];
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__startScreenRecording];
    [bridge deregisterHandler:k__stopScreenRecording];
    [bridge deregisterHandler:k__cancelScreenRecording];
    [bridge deregisterHandler:k__getScreenRecordingUrl];
    [bridge deregisterHandler:k__checkRecordingPermission];
}

- (void)startScreenRecording {
    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    [recorder
        startRecordingWithMicrophoneEnabled:YES
                                    handler:^(NSError* error) {
                                        if (error != nil) {
                                            NSLog(@"startScreenRecording: %@",
                                                  [error localizedDescription]);
                                        }
                                    }];
}

- (void)stopScreenRecording {
    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    [recorder stopRecordingWithHandler:^(
                  RPPreviewViewController* previewViewController,
                  NSError* error) {
        if (error != nil) {
            NSLog(@"stopScreenRecording: %@", [error localizedDescription]);
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

- (void)cancelScreenRecording {
    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    [recorder
        stopRecordingWithHandler:^(
            RPPreviewViewController* previewViewController, NSError* error) {
            if (error != nil) {
                NSLog(@"cancelRecordScreen: %@", [error localizedDescription]);
            }
        }];
}

- (BOOL)checkRecordingPermission {
    RPScreenRecorder* recorder = [RPScreenRecorder sharedRecorder];
    return [recorder isAvailable];
}

- (NSString*)getScreenRecordingUrl {
    return @"";
}

- (void)previewControllerDidFinish:(RPPreviewViewController*)previewController {
    [previewController dismissViewControllerAnimated:YES completion:nil];
}

@end
