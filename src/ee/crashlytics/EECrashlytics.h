//
//  EECrashlyticsProtocol.h
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEIPlugin.h"

@interface EECrashlytics : NSObject <EEIPlugin>

- (void)causeCrash;

- (void)causeException;

- (void)log:(NSString* _Nonnull)desc
        tag:(NSString* _Nonnull)tag
    message:(NSString* _Nonnull)message;

- (void)setString:(NSString* _Nonnull)key value:(NSString* _Nonnull)value;

- (void)setBool:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value;

- (void)setInt:(NSString* _Nonnull)key value:(NSNumber* _Nonnull)value;

- (void)setUserIdentifier:(NSString* _Nonnull)identifier;

- (void)setUserName:(NSString* _Nonnull)name;

- (void)setUserEmail:(NSString* _Nonnull)email;

- (void)trackLevelStart:(NSString* _Nonnull)name
       customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs;

- (void)trackLevelEnd:(NSString* _Nonnull)name
                score:(NSNumber* _Nonnull)score
              success:(NSNumber* _Nonnull)success
     customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs;

- (void)trackPurchaseWithPrice:(NSNumber* _Nonnull)price
                      currency:(NSString* _Nonnull)currency
                       success:(NSNumber* _Nonnull)success
                      itemName:(NSString* _Nonnull)itemName
                      itemType:(NSString* _Nonnull)itemType
                        itemId:(NSString* _Nonnull)itemId
              customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs;

- (void)trackCustomEvent:(NSString* _Nonnull)eventName
        customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs;

- (void)trackInviteWithMethod:(NSString* _Nonnull)method
             customAttributes:(NSDictionary<NSString*, id>* _Nonnull)attrs;
@end
