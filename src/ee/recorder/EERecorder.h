//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEIPlugin.h"

@interface EERecorder : NSObject <EEIPlugin>

- (void)startRecording;
- (void)stopRecording;
- (void)cancelRecording;
- (NSString* _Nonnull)getRecordingUrl;
- (BOOL)checkRecordingPermission;

@end
