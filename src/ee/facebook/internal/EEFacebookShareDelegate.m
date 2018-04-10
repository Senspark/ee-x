//
//  EEFacebookShareDelegate.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#import "ee/facebook/internal/EEFacebookShareDelegate.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/EEIMessageBridge.h"

@implementation EEFacebookShareDelegate {
    int tag_;
    id<EEIMessageBridge> bridge_;
}

+ (instancetype)delegateWithBridge:(id<EEIMessageBridge>)bridge tag:(int)tag {
    EEFacebookShareDelegate* delegate = [[[self alloc] init] autorelease];
    delegate->bridge_ = bridge;
    delegate->tag_ = tag;
    return delegate;
}

- (NSString* _Nonnull)k__onSuccess {
    return [@"FacebookShareDelegate_onSuccess_"
        stringByAppendingString:[@(tag_) stringValue]];
}

- (NSString* _Nonnull)k__onFailure {
    return [@"FacebookShareDelegate_onFailure_"
        stringByAppendingString:[@(tag_) stringValue]];
}

- (NSString* _Nonnull)k__onCancel {
    return [@"FacebookShareDelegate_onCancel_"
        stringByAppendingString:[@(tag_) stringValue]];
}

- (void)sharer:(id<FBSDKSharing>)sharer
    didCompleteWithResults:(NSDictionary*)results {
    [bridge_ callCpp:[self k__onSuccess]
             message:[EEJsonUtils convertDictionaryToString:results]];
    [self release];
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError*)error {
    [bridge_ callCpp:[self k__onFailure] message:[error localizedDescription]];
    [self release];
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer {
    [bridge_ callCpp:[self k__onCancel]];
    [self release];
}

@end
