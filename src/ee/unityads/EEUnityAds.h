//
//  EEUnityAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface EEUnityAds : NSObject

- (void) initUnityAds:(NSString*) gameID;

- (BOOL) isAdsReady:(NSString*) placementID;

- (void) showAds:(NSString*) placementID;


@property(assign, nonatomic) UIViewController* rootController;

@end
