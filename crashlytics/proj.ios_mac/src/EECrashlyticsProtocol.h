//
//  EECrashlyticsProtocol.h
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <EEPluginProtocol.h>

// clang-format off

@interface EECrashlyticsProtocol : EEPluginProtocol;

- (id _Null_unspecified) init;

- (void) causeCrash;

- (void) causeException;

- (void) log:(NSDictionary* _Nonnull) dict;

@end

// clang-format on