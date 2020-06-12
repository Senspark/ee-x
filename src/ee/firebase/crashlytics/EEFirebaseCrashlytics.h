@protocol EEIPlugin;

@interface EEFirebaseCrashlytics : NSObject <EEIPlugin>

- (void)log:(NSString*)message;

@end
