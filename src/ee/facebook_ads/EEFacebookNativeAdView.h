//
//  AdsUIView.h
//  Pods
//
//  Created by Pham Xuan Han on 5/16/17.
//
//

#import <UIKit/UIKit.h>

@class FBMediaView;
@class FBAdChoicesView;
@class FBAdIconView;

@interface EEFacebookNativeAdView : UIView

//@property (nonatomic, assign) IBOutlet UIImageView* iconImage;
@property (nonatomic, assign) IBOutlet UILabel* titleLabel;
@property (nonatomic, assign) IBOutlet UILabel* sponsorLabel;
@property (nonatomic, assign) IBOutlet FBAdIconView* mediaView;
//@property (nonatomic, assign) IBOutlet UIImageView* coverImage;
@property (nonatomic, assign) IBOutlet FBMediaView* iconView;
@property (nonatomic, assign) IBOutlet UIButton* callToActionButton;
@property (nonatomic, assign) IBOutlet UILabel* socialContextLabel;
@property (nonatomic, assign) IBOutlet UILabel* bodyLabel;
@property (nonatomic, assign) IBOutlet FBAdChoicesView* adchoicesView;

@end
