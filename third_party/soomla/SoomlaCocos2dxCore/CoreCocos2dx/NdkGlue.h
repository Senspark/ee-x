
#import <Foundation/Foundation.h>

@interface NdkGlue : NSObject
+ (NdkGlue*)sharedInstance;

- (void)registerCallHandlerForKey:(NSString*)key
                        withBlock:(void (^)(NSDictionary* parameters,
                                            NSMutableDictionary* retParameters))
                                      callHandler;
- (void)registerCallbackHandlerForKey:(NSString*)key
                            withBlock:
                                (void (^)(NSNotification* parameters,
                                          NSMutableDictionary* retParameters))
                                    callbackHandler;
- (void)registerExceptionHandlerForKey:(NSString*)key
                             withBlock:
                                 (void (^)(NSException* exception,
                                           NSDictionary* parameters,
                                           NSMutableDictionary* retParameters))
                                     exceptionHandler;

- (NSObject*)dispatchNdkCall:(NSDictionary*)parameters;
- (void)dispatchNdkCallback:(NSNotification*)notification;
@end
