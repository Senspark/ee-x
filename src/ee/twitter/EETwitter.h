//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEIPlugin.h"

@interface EETwitter : NSObject <EEIPlugin>

- (void)initialize;
- (void)shareContent:(NSString*)text;
- (void)shareScreenShot:(NSString*)text image:(NSString*)image;

@end
