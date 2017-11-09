//
//  EEStore.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <StoreKit/StoreKit.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/store/internal/EEStore.h"

@interface EEStore () <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@end

@implementation EEStore

// clang-format off
static NSString* const k__request_products_succeeded  = @"Store_requestProductSucceeded";
static NSString* const k__request_products_failed     = @"Store_requestProductFailed";
static NSString* const k__restore_purchases_succeeded = @"Store_restorePurchasesSucceeded";
static NSString* const k__restore_purchases_failed    = @"Store_restorePurchasesFailed";
static NSString* const k__transaction_succeeded       = @"Store_transactionSucceeded";
static NSString* const k__transaction_failed          = @"Store_transactionFailed";
static NSString* const k__transaction_restored        = @"Store_transactionRestored";
// clang-format on

// clang-format off
static NSString* const k__title             = @"title";
static NSString* const k__description       = @"description";
static NSString* const k__price             = @"price";
static NSString* const k__product_id        = @"product_id";
static NSString* const k__transaction_id    = @"transaction_id";
static NSString* const k__currency_symbol   = @"currency_symbol";
static NSString* const k__currency_code     = @"currency_code";
static NSString* const k__error_code        = @"error_code";
// clang-format on

+ (instancetype _Nullable)getInstance {
    static EEStore* sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[[self class] alloc] init];
    });
    return sharedInstance;
}

+ (BOOL)canMakePayments {
    return [SKPaymentQueue canMakePayments];
}

- (instancetype)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue addTransactionObserver:self];
    return self;
}

- (BOOL)initialize {
    return YES;
}

- (void)dealloc {
    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue removeTransactionObserver:self];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
}

- (void)retryUnfinishedTransactions {
    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    NSArray* transactions = [paymentQueue transactions];
    [self paymentQueue:paymentQueue updatedTransactions:transactions];
}

- (void)restoreTransactions {
    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue restoreCompletedTransactions];
}

- (void)addPayment:(NSString* _Nonnull)productIdentifier {
    SKMutablePayment* payment = [[[SKMutablePayment alloc] init] autorelease];
    [payment setProductIdentifier:productIdentifier];
    [payment setQuantity:1];
    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue addPayment:payment];
}

- (void)requestProducts:(NSSet* _Nonnull)identifiers {
    SKProductsRequest* request = [[[SKProductsRequest alloc]
        initWithProductIdentifiers:identifiers] autorelease];
    [request setDelegate:self];
    [request start];
}

- (void)productsRequest:(SKProductsRequest*)request
     didReceiveResponse:(SKProductsResponse*)response {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSMutableArray* arr = [NSMutableArray array];

    NSArray* products = [response products];
    for (SKProduct* product in products) {
        NSString* title = [product localizedTitle];
        NSString* description = [product localizedDescription];
        NSDecimalNumber* price = [product price];
        NSLocale* locale = [product priceLocale];
        NSString* productId = [product productIdentifier];

        NSDictionary* dict = @{
            k__title: title,
            k__description: description,
            k__price: @([price doubleValue]),
            k__product_id: productId,
            k__currency_symbol: [locale objectForKey:NSLocaleCurrencySymbol],
            k__currency_code: [locale objectForKey:NSLocaleCurrencyCode]
        };
        [arr addObject:dict];
    }

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__request_products_succeeded
            message:[EEJsonUtils convertArrayToString:arr]];
}

- (void)request:(SKRequest*)request didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__request_products_failed
            message:[@([error code]) stringValue]];
}

- (void)paymentQueue:(SKPaymentQueue*)queue
    updatedTransactions:(NSArray<SKPaymentTransaction*>*)transactions {
    for (SKPaymentTransaction* transaction in transactions) {
        switch ([transaction transactionState]) {
        case SKPaymentTransactionStatePurchasing:
            // Ignore.
            break;
        case SKPaymentTransactionStatePurchased:
            [self purchaseTransaction:transaction];
            break;
        case SKPaymentTransactionStateFailed:
            [self failTransaction:transaction];
            break;
        case SKPaymentTransactionStateRestored:
            [self restoreTransaction:transaction];
            break;
        case SKPaymentTransactionStateDeferred:
            [self deferTransaction:transaction];
            break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue*)queue
    removedTransactions:(NSArray<SKPaymentTransaction*>*)transactions {
    //
}

- (void)paymentQueue:(SKPaymentQueue*)queue
    restoreCompletedTransactionsFailedWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__restore_purchases_failed
            message:[error localizedDescription]];
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:
    (SKPaymentQueue*)queue {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__restore_purchases_succeeded];
}

- (void)purchaseTransaction:(SKPaymentTransaction*)transaction {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStatePurchased,
             @"Unexpected transaction state");

    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue finishTransaction:transaction];

    SKPayment* payment = [transaction payment];
    NSString* productId = [payment productIdentifier];
    NSString* transactionId = [transaction transactionIdentifier];
    NSDictionary* dict = @{
        k__product_id: productId,
        k__transaction_id: transactionId,
    };

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__transaction_succeeded
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)failTransaction:(SKPaymentTransaction*)transaction {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__,
          [[transaction error] localizedDescription]);
    NSAssert([transaction transactionState] == SKPaymentTransactionStateFailed,
             @"Unexpected transaction state");

    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue finishTransaction:transaction];

    SKPayment* payment = [transaction payment];
    NSString* productId = [payment productIdentifier];
    NSDictionary* dict = @{
        k__product_id: productId,
        k__error_code: @([[transaction error] code])
    };

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__transaction_failed
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)restoreTransaction:(SKPaymentTransaction*)transaction {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStateRestored,
             @"Unexpected transaction state");

    SKPaymentQueue* paymentQueue = [SKPaymentQueue defaultQueue];
    [paymentQueue finishTransaction:transaction];

    SKPayment* payment = [transaction payment];
    NSString* productId = [payment productIdentifier];
    NSString* transactionId = [transaction transactionIdentifier];
    NSDictionary* dict = @{
        k__product_id: productId,
        k__transaction_id: transactionId,
    };

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__transaction_restored
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)deferTransaction:(SKPaymentTransaction*)transaction {
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStateDeferred,
             @"Unexpected transaction state");
    // http://stackoverflow.com/questions/26187148/deferred-transactions
    BOOL fix_clang_format __unused;
}

@end
