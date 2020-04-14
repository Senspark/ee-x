//
//  EEBannerAdProtocol.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <CoreGraphics/CGGeometry.h>
#import <Foundation/NSObject.h>

@protocol EEIAdView <NSObject>

- (BOOL)isLoaded;
- (void)load;
- (CGPoint)getPosition;
- (void)setPosition:(CGPoint)position;
- (CGSize)getSize;
- (void)setSize:(CGSize)size;
- (BOOL)isVisible;
- (void)setVisible:(BOOL)visible;

@end
