#import "ee/firebase/crashlytics/private/EEFirebaseCrashlytics.h"

#import <FirebaseCrashlytics/FIRCrashlytics.h>

#import <ee/core/internal/EEMessageBridge.h>

@interface EEFirebaseCrashlytics () {
    id<EEIMessageBridge> bridge_;
}

@end

@interface EEFirebaseCrashlytics

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
    [bridge_ registerHandler:kLog
                    callback:^(NSString* message) {
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