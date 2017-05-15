//
//  EEStore.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <StoreKit/StoreKit.h>

#import "ee/store/internal/EEStore.h"

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

- (BOOL)initialize {

    return YES;
}

- (void)dealloc {
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

- (void)loadBillingService {
    if ([EEStore billingSupported]) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        // FIXME: postBillingSupported.
    } else {
        // FIXME: postBillingNotSupported.
    }
}

- (void)restoreTransactions {
    if ([EEStore billingSupported]) {
        [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    }
}

- (void)retryUnfinishedTransactions {
    NSArray* transactions = [[SKPaymentQueue defaultQueue] transactions];
    [self paymentQueue:[SKPaymentQueue defaultQueue]
        updatedTransactions:transactions];
}

- (BOOL)buyInMarketWithMarketItem:(NSString*)productId
                       andPayload:(NSString*)payload {
    if (![EEStore billingSupported]) {
        return NO;
    }
    SKMutablePayment* payment = [[[SKMutablePayment alloc] init] autorelease];
    [payment setProductIdentifier:productId];
    [payment setQuantity:1];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    return YES;
}

- (void)paymentQueue:(SKPaymentQueue*)queue
    updatedTransactions:(NSArray<SKPaymentTransaction*>*)transactions {
    for (SKPaymentTransaction* transaction in transactions) {
        switch ([transaction transactionState]) {
        case SKPaymentTransactionStatePurchasing:
            break;
        case SKPaymentTransactionStatePurchased:
            [self completeTransaction:transaction];
            break;
        case SKPaymentTransactionStateFailed:
            [self failedTransaction:transaction];
            break;
        case SKPaymentTransactionStateRestored:
            break;
        case SKPaymentTransactionStateDeferred:
            [self deferTransaction:transaction];
            break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction*)transaction {
    [self givePurchasedItem:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction*)transaction {
    if ([[transaction error] code] != SKErrorPaymentCancelled) {
        // FIXME: Unexpectetd error.
    } else {
        // FIXME: Market purchase canceled.
    }
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)deferTransaction:(SKPaymentTransaction*)transaction {
    // FIXME.
}

- (void)givePurchasedItem:(SKPaymentTransaction*)transaction {
    [self givePurchasedItem:transaction isRestored:NO];
}

- (void)givePurchasedItem:(SKPaymentTransaction*)transaction
               isRestored:(BOOL)isRestored {
    //
}

- (void)finalizeTransaction:(SKPaymentTransaction*)transaction
                 isRestored:(BOOL)isRestored
                  productId:(NSString*)productId {
}

@end
