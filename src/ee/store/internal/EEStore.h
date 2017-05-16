//
//  EEStore.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/SKPaymentQueue.h>
#import <StoreKit/SKProductsRequest.h>

@class SKPaymentTransaction;

@interface EEStore
    : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
}

+ (instancetype)getInstance;

+ (BOOL)canMakePayments;

- (void)addPayment:(NSString*)productIdentifier;

- (void)restoreTransactions;

@end
