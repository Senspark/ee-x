//
//  EECrashlyticsProtocol.h
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <ee/core/EEIPlugin.h>

@interface EECrashlytics : NSObject <EEIPlugin>

- (void)causeCrash;

- (void)causeException;

- (void)log:(NSString* _Nonnull)desc
        tag:(NSString* _Nonnull)tag
    message:(NSString* _Nonnull)message;

@end
