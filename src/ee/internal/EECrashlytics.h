//
//  EECrashlyticsProtocol.h
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import <Foundation/Foundation.h>

@interface EECrashlytics : NSObject

- (void)causeCrash;

- (void)causeException;

- (void)log:(NSString* _Nonnull)desc
        tag:(NSString* _Nonnull)tag
        msg:(NSString* _Nonnull)msg;

- (void)setString:(NSString* _Nonnull)key value:(NSString* _Nonnull)value;

- (void)setBool:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value;

- (void)setInt:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value;

- (void)setUserIdentifier:(NSString* _Nonnull)identifier;

- (void)setUserName:(NSString* _Nonnull)name;

- (void)setUserEmail:(NSString* _Nonnull)email;

@end
