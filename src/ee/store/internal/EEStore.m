//
//  EEStore.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <StoreKit/StoreKit.h>

#import "ee/store/internal/EEStore.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"

@implementation EEStore

+ (instancetype)getInstance {
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
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    return self;
}

- (BOOL)initialize {

    return YES;
}

- (void)dealloc {
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

- (void)retryUnfinishedTransactions {
    NSArray* transactions = [[SKPaymentQueue defaultQueue] transactions];
    [self paymentQueue:[SKPaymentQueue defaultQueue]
        updatedTransactions:transactions];
}

- (void)restoreTransactions {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void)addPayment:(NSString*)productIdentifier {
    SKMutablePayment* payment = [[[SKMutablePayment alloc] init] autorelease];
    [payment setProductIdentifier:productIdentifier];
    [payment setQuantity:1];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)requestProducts:(NSSet*)identifiers {
    SKProductsRequest* request = [[[SKProductsRequest alloc]
        initWithProductIdentifiers:identifiers] autorelease];
    [request setDelegate:self];
    [request start];
}

- (void)productsRequest:(SKProductsRequest*)request
     didReceiveResponse:(SKProductsResponse*)response {
    NSMutableArray* arr = [NSMutableArray alloc];

    NSArray* products = [response products];
    for (SKProduct* product in products) {
        NSString* title = [product localizedTitle];
        NSString* description = [product localizedDescription];
        NSDecimalNumber* price = [product price];
        NSLocale* locale = [product priceLocale];
        NSString* productId = [product productIdentifier];

        NSDictionary* dict = @{
            @"title": title,
            @"description": description,
            @"price": @([price doubleValue]),
            @"productId": productId,
            @"currencySymbol": [locale objectForKey:NSLocaleCurrencySymbol],
            @"currencyCode": [locale objectForKey:NSLocaleCurrencyCode]
        };
        [arr addObject:dict];
    }

    [[EEMessageBridge getInstance]
        callCpp:@"__store_products_request_succeeded"
            msg:[EEJsonUtils convertArrayToString:arr]];
}

- (void)request:(SKRequest*)request didFailWithError:(NSError*)error {
    NSDictionary* dict = [EEDictionaryUtils createDictWithError:error];
    [[EEMessageBridge getInstance]
        callCpp:@"__store_products_request_failed"
            msg:[EEJsonUtils convertDictionaryToString:dict]];
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
    NSDictionary* dict = [EEDictionaryUtils createDictWithError:error];
    [[EEMessageBridge getInstance]
        callCpp:@"__store_restore_transactions_failed"
            msg:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:
        (SKPaymentQueue*)queue {
    [[EEMessageBridge getInstance]
        callCpp:@"__store_restore_transactions_succeeded"
            msg:@""];
}

- (void)purchaseTransaction:(SKPaymentTransaction*)transaction {
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStatePurchased,
             @"..");

    NSString* productIdentifier = [[transaction payment] productIdentifier];
    NSDictionary* dict = @{
        @"productIdentifier": productIdentifier,
        @"transactionIdentifier": [transaction transactionIdentifier],
    };

    [[EEMessageBridge getInstance]
        callCpp:@"__store_transaction_purchased"
            msg:[EEJsonUtils convertDictionaryToString:dict]];

    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failTransaction:(SKPaymentTransaction*)transaction {
    NSAssert([transaction transactionState] == SKPaymentTransactionStateFailed,
             @"..");

    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [EEDictionaryUtils setError:[transaction error] forDict:dict];

    NSString* productIdentifier = [[transaction payment] productIdentifier];
    [dict setObject:productIdentifier forKey:@"productIdentifier"];

    [[EEMessageBridge getInstance]
        callCpp:@"__store_transaction_failed"
            msg:[EEJsonUtils convertDictionaryToString:dict]];

    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction*)transaction {
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStateRestored,
             @"..");

    NSString* productIdentifier = [[transaction payment] productIdentifier];
    NSDictionary* dict = @{
        @"productIdentifier": productIdentifier,
        @"transactionIdentifier": [transaction transactionIdentifier],
    };

    [[EEMessageBridge getInstance]
        callCpp:@"__store_transaction_restored"
            msg:[EEJsonUtils convertDictionaryToString:dict]];

    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)deferTransaction:(SKPaymentTransaction*)transaction {
    NSAssert([transaction transactionState] ==
                 SKPaymentTransactionStateDeferred,
             @"..");
    // http://stackoverflow.com/questions/26187148/deferred-transactions
    BOOL fix_clang_format __unused;
}

@end
