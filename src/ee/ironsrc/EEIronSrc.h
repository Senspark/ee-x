#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface EEIronSrc : NSObject

- (void) initIronSrc:(NSString*) gameID;

- (BOOL) isAdsReady:(NSString*) placementID;

- (void) showAds:(NSString*) placementID;

@property(assign, nonatomic) UIViewController* rootController;

@end
