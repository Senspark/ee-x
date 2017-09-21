//
//  EEALovinAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface EEALovinAds : NSObject

- (void) initALovinAds:(NSString*) gameID;

- (BOOL) isAdsReady:(NSString*) placementID;

- (void) showAds:(NSString*) placementID;


@property(assign, nonatomic) UIViewController* rootController;

@end
