//
//  EEFirebasePerformanceTrace.h
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import <Foundation/Foundation.h>
#import <FirebasePerformance/FIRTrace.h>

NS_ASSUME_NONNULL_BEGIN

@interface EEFirebasePerformanceTrace : NSObject

- (id _Nullable)initWithTraceName:(NSString* _Nonnull)traceName;
- (id _Nullable)initWithTraceName:(NSString* _Nonnull)traceName
                            trace:(FIRTrace*)trace;

- (void)start;
- (void)stop;
- (void)putMetric:(NSString* _Nonnull)metricName value:(int64_t)value;
- (void)incrementMetric:(NSString* _Nonnull)metricName value:(int64_t)value;
- (int64_t)getLongMetric:(NSString* _Nonnull)metricName;

@end

NS_ASSUME_NONNULL_END
