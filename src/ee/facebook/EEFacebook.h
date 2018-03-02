//
//  Created by Zinge on 7/4/16.
//
//

#import <Foundation/Foundation.h>

@interface EEFacebook : NSObject

- (void)shareLinkContent:(NSString* _Nonnull)url;
- (void)sharePhotoContent:(NSString* _Nonnull)url;
- (void)shareVideoContent:(NSString* _Nonnull)url;

@end
