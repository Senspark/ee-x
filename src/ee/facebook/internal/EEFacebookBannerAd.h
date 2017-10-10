//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <Foundation/NSObject.h>

@interface EEFacebookBannerAd : NSObject

+ (FBAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId size:(FBAdSize)adSize;

- (void)load;
- (BOOL)isLoaded;
- (CGPoint)getPosition;
- (void)setPosition:(CGPoint)position;
- (CGSize)getSize;
- (void)setSize:(CGSize)size;
- (void)setVisible:(BOOL)visible;

@end
