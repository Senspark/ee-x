#import "ee/firebase/crashlytics/EEFirebaseCrashlytics.h"

#import <ee_x-Swift.h>

#import <FirebaseCrashlytics/FIRCrashlytics.h>

@implementation EEFirebaseCrashlytics {
    id<EEIMessageBridge> bridge_;
}

static NSString* const kLog = @"FirebaseCrashlytics_log";

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:
                        kLog:^(NSString* message) {
                            [self log:message];
                            return @"";
                        }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:kLog];
}

- (void)log:(NSString*)message {
    [[FIRCrashlytics crashlytics] log:message];
}

@end
