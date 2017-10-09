//
//  AppController.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate>

@property (nonatomic, readonly) RootViewController* viewController;

@end
