//
//  EEStore.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/SKPaymentQueue.h>

@class SKPaymentTransaction;

typedef void (^EETransactionSuccess)(SKPaymentTransaction* transaction);
typedef void (^EETransactionFailure)(SKPaymentTransaction* transaction,
                                     NSError* error);

@interface EEStore : NSObject <SKPaymentTransactionObserver> {
}

+ (instancetype)getInstance;

+ (BOOL)canMakePayments;

- (void)addPayment:(NSString*)productIdentifier;

- (void)addPayment:(NSString*)productIdentifier
           success:(EETransactionSuccess)successBlock
           failure:(EETransactionFailure)failureBlock;

@end
