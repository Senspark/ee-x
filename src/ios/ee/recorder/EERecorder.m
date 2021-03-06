//
//  Created by Zinge on 7/4/16.
//
//

/*
#import "ee/recorder/EERecorder.h"

#import <ReplayKit/ReplayKit.h>

#import <ee-Swift.h>

#import <ee/core/EEJsonUtils.h>

@interface EERecorder () <RPScreenRecorderDelegate,
                          RPPreviewViewControllerDelegate> {
    id<EEIMessageBridge> bridge_;
    RPScreenRecorder* recorder_;
}

@end

@implementation EERecorder

#define kPrefix @"RecorderBridge"

// clang-format off
NSString* const k__isSupported     = kPrefix "IsSupported";
NSString* const k__startRecording  = kPrefix "StartRecording";
NSString* const k__stopRecording   = kPrefix "StopRecording";
NSString* const k__cancelRecording = kPrefix "CancelRecording";
NSString* const k__getRecordingUrl = kPrefix "GetRecordingUrl";
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
    [bridge_ registerHandler:
              k__isSupported:^(NSString* message) {
                  return [EEUtils toString:[self isSupported]];
              }];

    [bridge_ registerHandler:
           k__startRecording:^(NSString* message) {
               [self startRecording];
               return @"";
           }];

    [bridge_ registerHandler:
            k__stopRecording:^(NSString* message) {
                [self stopRecording];
                return @"";
            }];

    [bridge_ registerHandler:
          k__cancelRecording:^(NSString* message) {
              [self cancelRecording];
              return @"";
          }];

    [bridge_ registerHandler:
          k__getRecordingUrl:^(NSString* message) {
              return [self getRecordingUrl];
          }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__isSupported];
    [bridge_ deregisterHandler:k__startRecording];
    [bridge_ deregisterHandler:k__stopRecording];
    [bridge_ deregisterHandler:k__cancelRecording];
    [bridge_ deregisterHandler:k__getRecordingUrl];
}

- (BOOL)isSupported {
    return [recorder_ isAvailable];
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

- (NSString*)getRecordingUrl {
    return @"";
}

- (void)previewControllerDidFinish:(RPPreviewViewController*)previewController {
    [previewController dismissViewControllerAnimated:YES completion:nil];
}

@end
*/
