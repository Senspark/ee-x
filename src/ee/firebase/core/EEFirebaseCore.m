#import <ee/firebase/core/EEFirebaseCore.h>

#import <FirebaseCore/FIRApp.h>

@implementation EEFirebaseCore

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    [FIRApp configure];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

@end
