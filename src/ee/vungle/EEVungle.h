#import <Foundation/Foundation.h>

@class UIViewController;

@interface EEVungle : NSObject

- (void)initVungle:(NSString*)gameID placementIds: (NSArray*) placementArr;

- (BOOL)isAdReady:(NSString*)placementId;

- (void)showAds:(NSString*)placementId;

@property (assign, nonatomic) UIViewController* rootController;

@end
