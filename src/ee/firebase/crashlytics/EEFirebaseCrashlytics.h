#import <ee/core/EEIPlugin.h>

@interface EEFirebaseCrashlytics : NSObject <EEIPlugin>

- (void)log:(NSString*)message;

@end