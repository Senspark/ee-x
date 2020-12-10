//
//  EEGoogleAnalyticsTracker.hpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#import <Foundation/Foundation.h>

@interface EEGoogleAnalyticsTracker : NSObject

- (id _Nullable)initWithTrackingId:(NSString* _Nonnull)trackingId;

- (void)setParameter:(NSString* _Nonnull)key value:(NSString* _Nonnull)value;
- (void)setAllowIDFACollection:(BOOL)enabled;
- (void)send:(NSDictionary* _Nonnull)dict;

@end
