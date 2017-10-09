//
//  EEMetrics.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <CoreGraphics/CGBase.h>
#import <Foundation/NSObject.h>

@interface EEMetrics : NSObject

+ (void)initializeMetrics;

+ (CGFloat)getDensity;

@end
