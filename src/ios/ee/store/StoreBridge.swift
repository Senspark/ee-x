//
//  EEStore.swift
//  ee-x-6914a733
//
//  Created by eps on 6/2/20.
//

import StoreKit
import TPInAppReceipt

private let kPrefix = "StoreBridge"
private let kGetAppReceipt = "\(kPrefix)GetAppReceipt"
private let kParseAppReceipt = "\(kPrefix)ParseAppReceipt"
private let kCanMakePayments = "\(kPrefix)CanMakePayments"
private let kGetSimulateAskToBuy = "\(kPrefix)GetSimulateAskToBuy"
private let kSetSimulateAskToBuy = "\(kPrefix)SetSimulateAskToBuy"
private let kRetrieveProducts = "\(kPrefix)RetrieveProducts"
private let kPurchase = "\(kPrefix)Purchase"
private let kFinishTransaction = "\(kPrefix)FinishTransaction"
private let kRestoreTransactions = "\(kPrefix)RestoreTransactions"
private let kRefreshAppReceipt = "\(kPrefix)RefreshAppReceipt"
private let kAddTransactionObserver = "\(kPrefix)AddTransactionObserver"
private let kCallback = "\(kPrefix)Callback"

@objc(EEStoreBridge)
class StoreBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _unityPurchasing = StoreUnityPurchasing()

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        super.init()
        _unityPurchasing.messageCallback = { subject, payload, receipt, transactionId in
            self._bridge.callCpp(kCallback, EEJsonUtils.convertDictionary(toString: [
                "subject": subject,
                "payload": payload,
                "receipt": receipt,
                "transactionId": transactionId
            ]))
        }
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        _unityPurchasing.messageCallback = nil
    }

    private func registerHandlers() {
        _bridge.registerHandler(kGetAppReceipt) { _ in
            self._unityPurchasing.getAppReceipt()
        }
        _bridge.registerHandler(kParseAppReceipt) { message in
            guard let data = Data(base64Encoded: message, options: .ignoreUnknownCharacters) else {
                return ""
            }
            guard let receipt = try? InAppReceipt.receipt(from: data) else {
                return ""
            }
            let dict: [String: Any] = [
                "inAppPurchaseReceipts": receipt.purchases.map { [
                    "quantity": $0.quantity,
                    "productId": $0.productIdentifier,
                    "transactionId": $0.transactionIdentifier,
                    "originalTransactionIdentifier": $0.originalTransactionIdentifier,
                    "purchaseDate": $0.purchaseDate.timeIntervalSince1970,
                    "originalPurchaseDate": $0.originalPurchaseDate?.timeIntervalSince1970 ?? 0,
                    "subscriptionExpirationDate": $0.subscriptionExpirationDate?.timeIntervalSince1970 ?? 0,
                    "cancellationDate": $0.cancellationDate?.timeIntervalSince1970 ?? 0,
                    "isFreeTrial": $0.subscriptionTrialPeriod,
                    "productType": $0.productType.rawValue,
                    "isIntroductoryPricePeriod": $0.subscriptionIntroductoryPricePeriod
                ] },
                "bundleId": receipt.bundleIdentifier,
                "appVersion": receipt.appVersion,
                "expirationDate": receipt.expirationDate?.timeIntervalSince1970 ?? 0,
                "originalApplicationVersion": receipt.originalAppVersion,
                "receiptCreationDate": receipt.creationDate.timeIntervalSince1970
            ]
            return EEJsonUtils.convertDictionary(toString: dict)
        }
        _bridge.registerHandler(kCanMakePayments) { _ in
            Utils.toString(self._unityPurchasing.canMakePayments)
        }
        _bridge.registerHandler(kGetSimulateAskToBuy) { _ in
            Utils.toString(self._unityPurchasing.simulateAskToBuyEnabled)
        }
        _bridge.registerHandler(kSetSimulateAskToBuy) { message in
            self._unityPurchasing.simulateAskToBuyEnabled = Utils.toBool(message)
            return ""
        }
        _bridge.registerHandler(kRetrieveProducts) { message in
            let productDefinitions = self._unityPurchasing.deserializeProductDefinitions(message)
            let productIds = Set(productDefinitions.map { $0.storeSpecificId })
            self._unityPurchasing.requestProducts(productIds)
            return ""
        }
        _bridge.registerHandler(kPurchase) { message in
            let productDefinition = self._unityPurchasing.deserializeProductDefinition(message)
            self._unityPurchasing.purchaseProduct(productDefinition)
            return ""
        }
        _bridge.registerHandler(kFinishTransaction) { message in
            if message.count > 0 {
                self._unityPurchasing.finishTransaction(message)
            }
            return ""
        }
        _bridge.registerHandler(kRestoreTransactions) { _ in
            self._unityPurchasing.restorePurchases()
            return ""
        }
        _bridge.registerHandler(kRefreshAppReceipt) { _ in
            self._unityPurchasing.refreshReceipt()
            return ""
        }
        _bridge.registerHandler(kAddTransactionObserver) { _ in
            self._unityPurchasing.addTransactionObserver()
            return ""
        }
    }

    private func deregisterHandlers() {
        _bridge.deregisterHandler(kGetAppReceipt)
        _bridge.deregisterHandler(kParseAppReceipt)
        _bridge.deregisterHandler(kCanMakePayments)
        _bridge.deregisterHandler(kGetSimulateAskToBuy)
        _bridge.deregisterHandler(kSetSimulateAskToBuy)
        _bridge.deregisterHandler(kRetrieveProducts)
        _bridge.deregisterHandler(kPurchase)
        _bridge.deregisterHandler(kFinishTransaction)
        _bridge.deregisterHandler(kRestoreTransactions)
        _bridge.deregisterHandler(kRefreshAppReceipt)
        _bridge.deregisterHandler(kAddTransactionObserver)
    }
}
