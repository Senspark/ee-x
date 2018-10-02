//
//  Created by Zinge on 7/4/16.
//
//

#import "ee/core/EEIPlugin.h"

@interface EETwitter : NSObject <EEIPlugin, TWTRComposerViewControllerDelegate>

- (void)initialize:(NSString*)key secret:(NSString*)secret;
- (void)shareContent:(NSString*)text;
- (void)shareScreenShot:(NSString*)text image:(NSString*)image;

@end
