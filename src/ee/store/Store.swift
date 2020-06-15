//
//  EEStore.swift
//  ee-x-6914a733
//
//  Created by eps on 6/2/20.
//

import Foundation
import StoreKit

private class StoreProductsRequestDelegate: NSObject, SKProductsRequestDelegate {
    private let _bridge: IMessageBridge

    fileprivate init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
    }

    fileprivate func productsRequest(_ request: SKProductsRequest, didReceive response: SKProductsResponse) {
        print(String(format: "%s", #function))
        let array = response.products.map { [
            "product_identifier": $0.productIdentifier,
            "localized_descrption": $0.localizedDescription,
            "localized_title": $0.localizedTitle,
            "price": $0.price.stringValue,
            "subscription_group_identifier": $0.subscriptionGroupIdentifier ?? "",
            "subscription_period_number_of_units": $0.subscriptionPeriod?.numberOfUnits ?? 0,
            "subscription_period_unit": $0.subscriptionPeriod?.unit ?? 0,
            "currency_symbol": $0.priceLocale.currencySymbol ?? "",
            "currency_code": $0.priceLocale.currencyCode ?? "",
        ] }

//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//        [bridge callCpp:k__request_products_succeeded
//                message:[EEJsonUtils convertArrayToString:arr]];
    }

    fileprivate func requestDidFinish(_ request: SKRequest) {
        // OK.
    }

    fileprivate func request(_ request: SKRequest, didFailWithError error: Error) {
//            NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//        [bridge callCpp:k__request_products_failed
//                message:[@([error code]) stringValue]];
    }
}

@objc(EEStore)
public class Store: NSObject, IPlugin, SKPaymentTransactionObserver {
    private let _bridge: IMessageBridge
    private let _queue: SKPaymentQueue

    public required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        _queue = SKPaymentQueue.default()
        super.init()
        _queue.add(self)
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        _queue.remove(self)
    }

    private func registerHandlers() {}

    private func deregisterHandlers() {}

    private func canMakePayments() -> Bool {
        return SKPaymentQueue.canMakePayments()
    }

    private func requestProducts(_ identifiers: [String]) {
        let delegate = StoreProductsRequestDelegate(_bridge)
        let request = SKProductsRequest(productIdentifiers: Set(identifiers))
        request.delegate = delegate
        request.start()
    }

    private func addPayment(_ productIdentifier: String) {
//        let payment = SKMutablePayment
    }

    private func restoreTransactions() {
        _queue.restoreCompletedTransactions()
    }

    public func paymentQueue(_ queue: SKPaymentQueue, updatedTransactions transactions: [SKPaymentTransaction]) {
        for transaction in transactions {
            switch transaction.transactionState {
            case .purchasing:
                // Ignored.
                break
            case .purchased:
                purchaseTransaction(transaction)
            case .failed:
                failTransaction(transaction)
            case .restored:
                restoreTransaction(transaction)
            case .deferred:
                deferTransaction(transaction)
                break
            @unknown default:
                fatalError()
            }
        }
        //
    }

    private func purchaseTransaction(_ transaction: SKPaymentTransaction) {
        assert(transaction.transactionState == .purchased)
        _queue.finishTransaction(transaction)

        let payment = transaction.payment
        let dict: [String: String] = [
            "product_id": payment.productIdentifier,
            "transaction_id": transaction.transactionIdentifier ?? "",
        ]

//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//        [bridge callCpp:k__transaction_succeeded
//                message:[EEJsonUtils convertDictionaryToString:dict]];
    }

    private func failTransaction(_ transaction: SKPaymentTransaction) {
        assert(transaction.transactionState == .failed)
        _queue.finishTransaction(transaction)

        let payment = transaction.payment
        let dict: [String: String] = [
            "product_id": payment.productIdentifier,
            "error": transaction.error?.localizedDescription ?? "",
        ]

//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//        [bridge callCpp:k__transaction_failed
//                message:[EEJsonUtils convertDictionaryToString:dict]];
    }

    private func restoreTransaction(_ transaction: SKPaymentTransaction) {
        assert(transaction.transactionState == .restored)
        _queue.finishTransaction(transaction)

        let payment = transaction.payment
        let dict: [String: String] = [
            "product_id": payment.productIdentifier,
            "transaction_id": transaction.transactionIdentifier ?? "",
        ]

//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//         [bridge callCpp:k__transaction_restored
//                 message:[EEJsonUtils convertDictionaryToString:dict]];
    }

    private func deferTransaction(_ transaction: SKPaymentTransaction) {
        assert(transaction.transactionState == .deferred)
        _queue.finishTransaction(transaction)

        // FIXME.
        // http://stackoverflow.com/questions/26187148/deferred-transactions
    }

    public func paymentQueue(_ queue: SKPaymentQueue, removedTransactions transactions: [SKPaymentTransaction]) {
        // FIXME.
    }

    public func paymentQueueRestoreCompletedTransactionsFinished(_ queue: SKPaymentQueue) {
        // FIXME.
//    EEMessageBridge* bridge = [EEMessageBridge getInstance];
//    [bridge callCpp:k__restore_purchases_succeeded];
    }

    public func paymentQueue(_ queue: SKPaymentQueue, restoreCompletedTransactionsFailedWithError error: Error) {
        // FIXME.
//        EEMessageBridge* bridge = [EEMessageBridge getInstance];
//        [bridge callCpp:k__restore_purchases_failed
//                message:[@([error code]) stringValue]];
    }
}
