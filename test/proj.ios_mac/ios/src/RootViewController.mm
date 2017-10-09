//
//  RootViewController.cpp
//  ee_x_test
//
//  Created by Zinge on 10/9/17.
//
//

#import "RootViewController.h"

#import <cocos2d.h>
#import <platform/ios/CCEAGLView-ios.h>

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller
 programmatically and want to perform customization that is not appropriate for
 viewDidLoad.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle
 *)nibBundleOrNil
 {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

// Implement loadView to create a view hierarchy programmatically, without using
// a nib.
- (void)loadView {
    // Initialize the CCEAGLView.
    CCEAGLView* eaglView = [CCEAGLView
             viewWithFrame:[[UIScreen mainScreen] bounds]
               pixelFormat:(__bridge NSString*)cocos2d::GLViewImpl::_pixelFormat
               depthFormat:(GLuint)cocos2d::GLViewImpl::_depthFormat
        preserveBackbuffer:NO
                sharegroup:nil
             multiSampling:NO
           numberOfSamples:0];

    // Enable or disable multiple touches.
    [eaglView setMultipleTouchEnabled:NO];

    // Set EAGLView as view of RootViewController.
    [self setView:eaglView];
}

// Implement viewDidLoad to do additional setup after loading the view,
// typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [super viewDidAppear:animated];
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)viewWillDisappear:(BOOL)animated {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [super viewWillDisappear:animated];
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)viewDidDisappear:(BOOL)animated {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [super viewDidDisappear:animated];
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAllButUpsideDown;
}
#endif // __IPHONE_6_0

- (BOOL)shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:
    (UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if (glview != nullptr) {
        CCEAGLView* eaglview = (__bridge CCEAGLView*)glview->getEAGLView();
        if (eaglview != nil) {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged(
                (int)s.width, (int)s.height);
        }
    }
}

// fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)didReceiveMemoryWarning {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

@end
