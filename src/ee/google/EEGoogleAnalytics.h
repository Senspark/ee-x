//
//  EEGoogleAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

@protocol EEIPlugin;

@interface EEGoogleAnalytics : NSObject <EEIPlugin>

- (void)setDispatchInterval:(NSTimeInterval)interval;
- (void)setDryRun:(BOOL)enabled;
- (void)setOptOut:(BOOL)enabled;
- (void)setTrackUncaughtException:(BOOL)enabled;
- (void)dispatch;
- (BOOL)createTracker:(NSString* _Nonnull)trackingId;
- (BOOL)destroyTracker:(NSString* _Nonnull)trackingId;

@end
