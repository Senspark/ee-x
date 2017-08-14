#import <Foundation/Foundation.h>

@class UIViewController;

@interface EEIronSrc : NSObject

- (void)initIronSrc:(NSString*)gameId;

- (BOOL)isAdsReady:(NSString*)placementId;

- (void)showAds:(NSString*)placementId;

@property (assign, nonatomic) UIViewController* rootController;

@end
