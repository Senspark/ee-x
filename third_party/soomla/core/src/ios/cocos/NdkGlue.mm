
#import "CCNdkBridgeIos.h"
#import "NdkGlue.h"

@interface NdkGlue ()
@property (nonatomic, retain) NSMutableDictionary* callHandlers;
@property (nonatomic, retain) NSMutableDictionary* callbackHandlers;
@property (nonatomic, retain) NSMutableDictionary* exceptionHandlers;
@end

@implementation NdkGlue {
}

static NSString* TAG = @"SOOMLA NdkGlue";

+ (NdkGlue*)sharedInstance {
    static NdkGlue* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (id)init {
    self = [super init];
    if (self) {
        self.callHandlers = [NSMutableDictionary dictionary];
        self.callbackHandlers = [NSMutableDictionary dictionary];
        self.exceptionHandlers = [NSMutableDictionary dictionary];
    }

    return self;
}

- (NSObject*)dispatchNdkCall:(NSDictionary*)parameters {
    NSString* methodName = [parameters objectForKey:@"method"];

    NSMutableDictionary* retParameters = [NSMutableDictionary dictionary];

    void (^callHandler)(NSDictionary*, NSDictionary*) =
        [self.callHandlers objectForKey:methodName];

    if (callHandler) {
        @try {
            callHandler(parameters, retParameters);
        } @catch (NSException* e) {
            void (^exceptionHandler)(NSException*, NSDictionary*,
                                     NSDictionary*) = [self.exceptionHandlers
                objectForKey:NSStringFromClass([e class])];

            if (exceptionHandler) {
                exceptionHandler(e, parameters, retParameters);
            } else {
                [self
                    logErrorWithTag:TAG
                         andMessage:[NSString
                                        stringWithFormat:@"Unhandled exception "
                                                         @"%@ in method %@",
                                                         e, methodName]];
            }
        }
    } else {
        [self
            logErrorWithTag:TAG
                 andMessage:[NSString stringWithFormat:@"Unsupported method %@",
                                                       methodName]];
    }
    return retParameters;
}

- (void)dispatchNdkCallback:(NSNotification*)notification {
    if (notification.object == self) {
        return;
    }

    NSMutableDictionary* parameters = [NSMutableDictionary dictionary];

    void (^callbackHandler)(NSNotification*, NSDictionary*) =
        [self.callbackHandlers objectForKey:notification.name];

    if (callbackHandler) {
        callbackHandler(notification, parameters);
    } else {
        [self logErrorWithTag:TAG
                   andMessage:[NSString
                                  stringWithFormat:@"Unknown notification %@",
                                                   notification.name]];
        return;
    }

    json_t* jsonPrms = nullptr;

    if (parameters != nil) {
        NSError* error = nil;
        NSData* jsonData =
            [NSJSONSerialization dataWithJSONObject:parameters
                                            options:NSJSONWritingPrettyPrinted
                                              error:&error];

        if (error != nil)
            return;

        NSString* jsonPrmsString =
            [[NSString alloc] initWithData:jsonData
                                  encoding:NSUTF8StringEncoding];

        json_error_t jerror;
        jsonPrms = json_loads([jsonPrmsString UTF8String], 0, &jerror);

        if (!jsonPrms) {
            [self
                logErrorWithTag:TAG
                     andMessage:[NSString
                                    stringWithFormat:@"error: at line #%d: %s",
                                                     jerror.line, jerror.text]];
            return;
        }
    }

    soomla::CCNdkBridgeIos::ndkCallback(jsonPrms);
    if (jsonPrms) {
        json_decref(jsonPrms);
    }
}

- (void)registerCallHandlerForKey:(NSString*)key
                        withBlock:(void (^)(NSDictionary* parameters,
                                            NSMutableDictionary* retParameters))
                                      callHandler {
    [[self callHandlers] setObject:callHandler forKey:key];
}

- (void)registerCallbackHandlerForKey:(NSString*)key
                            withBlock:
                                (void (^)(NSNotification* parameters,
                                          NSMutableDictionary* retParameters))
                                    callbackHandler {
    [[self callbackHandlers] setObject:callbackHandler forKey:key];
}

- (void)registerExceptionHandlerForKey:(NSString*)key
                             withBlock:
                                 (void (^)(NSException* exception,
                                           NSDictionary* parameters,
                                           NSMutableDictionary* retParameters))
                                     exceptionHandler {
    [[self exceptionHandlers] setObject:exceptionHandler forKey:key];
}

- (void)logErrorWithTag:(NSString*)tag andMessage:(NSString*)message {
    NSLog(@"[*** ERROR ***] %@: %@", tag, message);
}

@end
