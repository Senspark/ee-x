//
//  EECrashlyticsProtocol.h
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <ee/internal/EEPluginProtocol.h>

// clang-format off

@interface EECrashlyticsProtocol : EEPluginProtocol;

- (id _Null_unspecified) init;

- (void) causeCrash;

- (void) causeException;

- (void) log:(NSDictionary* _Nonnull) dict;

- (void) setString:(NSDictionary* _Nonnull) dict;

- (void) setBool:(NSDictionary* _Nonnull) dict;

- (void) setInt:(NSDictionary* _Nonnull) dict;

- (void) setUserIdentifier:(NSDictionary* _Nonnull) dict;

- (void) setUserName:(NSDictionary* _Nonnull) dict;

- (void) setUserEmail:(NSDictionary* _Nonnull) dict;

@end

// clang-format on
