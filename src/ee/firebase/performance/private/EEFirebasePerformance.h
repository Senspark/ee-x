//
//  EEFirebasePerformance.h
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/9/19.
//

#import <ee/core/EEIPlugin.h>

NS_ASSUME_NONNULL_BEGIN

@interface EEFirebasePerformance : NSObject <EEIPlugin>

- (void) setDataCollectionEnabled:(bool) enabled;
- (BOOL) isDataCollectionEnabled;
- (BOOL) startTrace: (NSString* _Nonnull) traceName;
- (BOOL) newTrace: (NSString* _Nonnull) traceName;

@end

NS_ASSUME_NONNULL_END
