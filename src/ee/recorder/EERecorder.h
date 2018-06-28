//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEIPlugin.h"

@interface EERecorder : NSObject <EEIPlugin>

- (void)startScreenRecording;
- (void)stopScreenRecording;
- (void)cancelScreenRecording;
- (NSString* _Nonnull)getScreenRecordingUrl;
- (BOOL)checkRecordingPermission;

@end
