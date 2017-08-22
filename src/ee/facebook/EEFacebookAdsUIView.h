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

@interface EEFacebookAdsUIView : UIView

@property (unsafe_unretained, nonatomic) IBOutlet UIImageView* iconImg;
@property (unsafe_unretained, nonatomic) IBOutlet UILabel* titleLbl;
@property (unsafe_unretained, nonatomic) IBOutlet UILabel* sponsorLbl;
@property (unsafe_unretained, nonatomic) IBOutlet FBMediaView* mediaView;
@property (unsafe_unretained, nonatomic) IBOutlet UIImageView* coverImg;
@property (unsafe_unretained, nonatomic) IBOutlet UIButton* callToAction;
@property (unsafe_unretained, nonatomic) IBOutlet UILabel* socialContext;
@property (unsafe_unretained, nonatomic) IBOutlet UILabel* bodyLbl;
@property (unsafe_unretained, nonatomic)
    IBOutlet FBAdChoicesView* adchoicesView;

@end
