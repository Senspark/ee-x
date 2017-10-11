//
//  RootViewController.hpp
//  ee_x_test
//
//  Created by Zinge on 10/9/17.
//
//

#import <UIKit/UIKit.h>

@interface RootViewController : UIViewController

@property (nonatomic, readonly, getter=isActive) BOOL active;

- (BOOL)prefersStatusBarHidden;

@end
