#import <FBAudienceNetwork/FBAdSize.h>

@interface EEFacebookBannerHelper : NSObject

- (FBAdSize)getAdSize:(int)index;
- (CGSize)getSize:(int)index;

@end
