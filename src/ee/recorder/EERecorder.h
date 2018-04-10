//
//  Created by Zinge on 7/4/16.
//
//

#import <Foundation/Foundation.h>

@interface EERecorder : NSObject

- (void)startScreenRecording;
- (void)stopScreenRecording;
- (void)cancelScreenRecording;
- (NSString* _Nonnull)getScreenRecordingUrl;
- (BOOL)checkRecordingPermission;

@end
