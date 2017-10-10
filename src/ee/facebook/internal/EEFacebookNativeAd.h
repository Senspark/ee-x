//
//  EEFacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <Foundation/Foundation.h>

@interface EEFacebookNativeAd : NSObject

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId
                     layout:(NSString* _Nonnull)layoutName;

- (BOOL)isLoaded;
- (void)load;
- (CGPoint)getPosition;
- (void)setPosition:(CGPoint)position;
- (CGSize)getSize;
- (void)setSize:(CGSize)size;
- (void)setVisible:(BOOL)visible;

@end
