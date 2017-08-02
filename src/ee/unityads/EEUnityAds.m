//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/unityads/EEUnityAds.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import <UnityAds/UnityAds.h>


#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@interface EEUnityAds () <UnityAdsDelegate> {
}


@end

@implementation EEUnityAds

NSString* const k__unityads_initUnityAds = @"k__unityads_initUnityAds";
NSString* const k__unityads_isAdsReady = @"k__unityads_isAdsReady";
NSString* const k__unityads_showAds = @"k__unityads_showAds";

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* GameID = dict[@"GameID"];

        [self initUnityAds:GameID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__unityads_initUnityAds];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        
        NSString* PlacementID = dict[@"PlacementID"];
        
        return ([self isAdsReady:PlacementID]) ? @"true" : @"false";
    } tag:k__unityads_isAdsReady];
    
    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        
        NSString* PlacementID = dict[@"PlacementID"];
        
        [self showAds:PlacementID];
        
        return [EEDictionaryUtils emptyResult];
    } tag:k__unityads_showAds];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__unityads_initUnityAds];
    [bridge deregisterHandler:k__unityads_isAdsReady];
    [bridge deregisterHandler:k__unityads_showAds];
}
#pragma mark ===================CODE HERE
- (void) initUnityAds:(NSString*) gameID
{
    [UnityAds initialize:gameID delegate:self];
//    [UnityAds setDebugMode:YES];
    
    _rootController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
}

- (BOOL) isAdsReady:(NSString*) placementID
{
    return [UnityAds isReady:placementID];
}

- (void) showAds:(NSString*) placementID
{
    if ([self isAdsReady:placementID]) {
        [UnityAds show:_rootController placementId:placementID];
    }
}
#pragma mark ===================UnityAdsDelegate
- (void)unityAdsReady:(NSString *)placementId{
    NSLog(@"EEUNITY ADS   ready %@", placementId);
}

- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString *)message{
    NSLog(@"EEUNITY ADS   error %@", message);
}

- (void)unityAdsDidStart:(NSString *)placementId{
}

- (void)unityAdsDidFinish:(NSString *)placementId withFinishState:(UnityAdsFinishState)state
{
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    [dict setValue:[NSNumber numberWithInteger:state] forKey:@"code"];
    [dict setValue:placementId forKey:@"placement"];
    
    NSLog(@"EEUNITY ADS   finish dict %@", [EEJsonUtils convertDictionaryToString:dict]);
    [[EEMessageBridge getInstance] callCpp:@"__UnityAds_callback" msg:[EEJsonUtils convertDictionaryToString:dict]];
}
@end
