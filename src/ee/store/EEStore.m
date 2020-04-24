//
//  EEStore.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import "ee/store/EEStore.h"

#import <ReactiveObjC/ReactiveObjC.h>
#import <StoreKit/StoreKit.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

#define kPrefix @"Store"

// clang-format off
static NSString* const k__can_purchase                = kPrefix "_canPurchase";
static NSString* const k__purchase                    = kPrefix "_purchase";
static NSString* const k__restore_transactions        = kPrefix "_restoreTransactions";
static NSString* const k__request_products            = kPrefix "_requestProducts";
static NSString* const k__request_products_succeeded  = kPrefix "_requestProductSucceeded";
static NSString* const k__request_products_failed     = kPrefix "_requestProductFailed";
static NSString* const k__restore_purchases_succeeded = kPrefix "_restorePurchasesSucceeded";
static NSString* const k__restore_purchases_failed    = kPrefix "_restorePurchasesFailed";
static NSString* const k__transaction_succeeded       = kPrefix "_transactionSucceeded";
static NSString* const k__transaction_failed          = kPrefix "_transactionFailed";
static NSString* const k__transaction_restored        = kPrefix "_transactionRestored";
// clang-format on

#undef kPrefix

@interface EEStoreProductsRequestDelegate : NSObject <SKProductsRequestDelegate>
@end

@implementation EEStoreProductsRequestDelegate {
    EEIMessageBridge* bridge_;
}

- (id)initWithBridge:(id<EEIMessageBridge>)bridge {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = bridge;
    return self;
}

- (void)productsRequest:(SKProductsRequest*)request
     didReceiveResponse:(SKProductsResponse*)response {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSMutableArray* arr = [NSMutableArray array];

    NSArray* products = [response products];
    for (SKProduct* product in products) {
        NSDictionary* dict = @{
            @"product_id": [product productIdentifier],
            @"localized_description": [product localizedDescription],
            @"localized_title": [product localizedTitle],
            @"price": @([[product price] longLongValue]),
            @"price_locale"
            //            k__currency_symbol: [locale
            //            objectForKey:NSLocaleCurrencySymbol],
            //            k__currency_code: [locale
            //            objectForKey:NSLocaleCurrencyCode]
        };
        [arr addObject:dict];
    }

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__request_products_succeeded
            message:[EEJsonUtils convertArrayToString:arr]];
}

- (void)requestDidFinish:(SKRequest*)request {
    // OK.
}

- (void)request:(SKRequest*)request didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__request_products_failed
            message:[@([error code]) stringValue]];
}

@end

@interface EEStore () <SKPaymentTransactionObserver>
@end

@implementation EEStore {
    EEIMessageBridge* bridge_;
    SKPaymentQueue* queue_;
}

- (instancetype)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }

    bridge_ = [EEMessageBridge getInstance];
    queue_ = [SKPaymentQueue defaultQueue];

    [self registerHandlers];
    [queue_ addTransactionObserver:self];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    [queue_ removeTransactionObserver:self];
    [self deregisterHandlers];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__can_purchase
                   callback:^(NSString* message) {
                       return [EEUtils toString:[EEStore canMakePayments]];
                   }];

    [bridge registerHandler:k__purchase
                   callback:^(NSString* message) {
                       NSString* productId = message;
                       [self addPayment:productId];
                       return @"";
                   }];

    [bridge registerHandler:k__restore_transactions
                   callback:^(NSString* message) {
                       [self restoreTransactions];
                       return @"";
                   }];

    [bridge registerHandler:k__request_products
                   callback:^(NSString* message) {
                       NSArray* productIds =
                           [EEJsonUtils convertStringToArray:message];
                       [self requestProducts:[NSSet setWithArray:productIds]];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__can_purchase];
    [bridge deregisterHandler:k__purchase];
}

- (BOOL)initialize {
    return YES;
}

- (BOOL)canMakePayments {
    return [SKPaymentQueue canMakePayments];
}

- (void)requestProducts:(NSSet* _Nonnull)identifiers {
    EEStoreProductsRequestDelegate* delegate =
        [[[EEStoreProductsRequestDelegate alloc] initWithBridge:bridge_]
            autorelease];

    SKProductsRequest* request = [[[SKProductsRequest alloc]
        initWithProductIdentifiers:identifiers] autorelease];
    [request setDelegate:delegate];
    [request start];
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

#pragma mark - SKPaymentTransactionObserver

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
            message:[@([error code]) stringValue]];
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:
    (SKPaymentQueue*)queue {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__restore_purchases_succeeded];
}

@end
