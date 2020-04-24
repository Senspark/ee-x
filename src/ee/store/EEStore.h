//
//  EEStore.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#import <Foundation/Foundation.h>

@class SKPaymentTransaction;

@interface EEStore : NSObject

- (void)destroy;

- (BOOL)canMakePayments;

- (void)addPayment:(NSString* _Nonnull)productIdentifier;

- (void)requestProducts:(NSSet* _Nonnull)identifiers;

- (void)restoreTransactions;

@end
