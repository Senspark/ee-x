//
//  StoreUnityPurchasing.swift
//  Pods
//
//  Created by eps on 6/29/20.
//

import StoreKit

internal typealias UnityPurchasingCallback = (_ subject: String,
                                              _ payload: String,
                                              _ receipt: String,
                                              _ transactionId: String) -> Void

internal class StoreUnityPurchasing: NSObject, SKProductsRequestDelegate, SKPaymentTransactionObserver {
    private let _queue = SKPaymentQueue.default()
    private var _messageCallback: UnityPurchasingCallback?
    private var _validProducts: [String: SKProduct] = [:]
    private var _productIds: Set<String>?
    private var _request: SKProductsRequest?
    private var _pendingTransactions: [String: SKPaymentTransaction] = [:]
    private var _finishedTransactions: Set<String> = []
    private var _receiptRefresher: StoreReceiptRefresher?
    private var _refreshRequest: SKReceiptRefreshRequest?
    private var _simulateAskToBuyEnabled = false
    private var _delayInSeconds = 2

    var messageCallback: UnityPurchasingCallback? {
        get { return _messageCallback }
        set(value) { _messageCallback = value }
    }

    var simulateAskToBuyEnabled: Bool {
        get { return _simulateAskToBuyEnabled }
        set(value) { _simulateAskToBuyEnabled = value }
    }

    func getAppReceipt() -> String {
        let bundle = Bundle.main
        if let receipUrl = bundle.appStoreReceiptURL {
            if FileManager.default.fileExists(atPath: receipUrl.path) {
                do {
                    let receipt = try Data(contentsOf: receipUrl)
                    let result = receipt.base64EncodedString()
                    return result
                } catch {
                    print("\(#function): \(error.localizedDescription)")
                }
            }
        }
        print("No App Receipt found")
        return ""
    }

    var canMakePayments: Bool {
        return SKPaymentQueue.canMakePayments()
    }

    private func unitySendMessage(_ subject: String, _ payload: String) {
        _messageCallback?(subject, payload, "", "")
    }

    private func unitySendMessage(_ subject: String, _ payload: String, _ receipt: String) {
        _messageCallback?(subject, payload, receipt, "")
    }

    private func unitySendMessage(_ subject: String, _ payload: String, _ receipt: String, _ transactionId: String) {
        _messageCallback?(subject, payload, receipt, transactionId)
    }

    func refreshReceipt() {
        let refresher = StoreReceiptRefresher { success in
            print("\(#function): RefreshReceipt status \(success)")
            if success {
                self.unitySendMessage("onAppReceiptRefreshed", self.getAppReceipt())
            } else {
                self.unitySendMessage("onAppReceiptRefreshFailed", "")
            }
        }
        let request = SKReceiptRefreshRequest()
        request.delegate = refresher
        request.start()
        _receiptRefresher = refresher
        _refreshRequest = request
    }

    /// Handle a new or restored purchase transaction by informing Unity.
    private func onTransactionSucceeded(_ transaction: SKPaymentTransaction) {
        let transactionId = transaction.transactionIdentifier ?? UUID().uuidString

        // This transaction was marked as finished, but was not cleared from the queue.
        // Try to clear it now, then pass the error up the stack as a DuplicateTransaction
        if _finishedTransactions.contains(transactionId) {
            _queue.finishTransaction(transaction)
            let productId = transaction.payment.productIdentifier
            print("\(#function): DuplicateTransaction error with product \(productId) and transactionId \(transactionId)")
            onPurchaseFailed(productId,
                             "DuplicateTransaction",
                             "",
                             "Duplicate transaction occurred")
            return
        }

        // Item was successfully purchased or restored.
        if _pendingTransactions.index(forKey: transactionId) == nil {
            _pendingTransactions[transactionId] = transaction
        }

        unitySendMessage("OnPurchaseSucceeded",
                         transaction.payment.productIdentifier,
                         getAppReceipt(),
                         transactionId)
    }

    /// Called back by managed code when the tranaction has been logged.
    func finishTransaction(_ transactionId: String) {
        if let transaction = _pendingTransactions[transactionId] {
            print("\(#function): Finishing transaction \(transactionId)")
            _queue.finishTransaction(transaction)
            _pendingTransactions.removeValue(forKey: transactionId)
            _finishedTransactions.insert(transactionId)
        } else {
            print("\(#function): Transaction \(transactionId) not pending, nothing to finish here")
        }
    }

    /// Request information about our products from Apple.
    func requestProducts(_ paramIds: Set<String>) {
        _productIds = paramIds
        print("\(#function): Requesting \(paramIds.count) products")

        // Start an immediate poll.
        initiateProductPoll(0)
    }

    /// Execute a product metadata retrieval request via GCD.
    private func initiateProductPoll(_ delayInSeconds: Int) {
        Thread.runOnMainThreadDelayed(Float(delayInSeconds)) {
            print("\(#function): Requesting product data...")
            let request = SKProductsRequest(productIdentifiers: self._productIds ?? [])
            request.delegate = self
            request.start()
            self._request = request
        }
    }

    func purchaseProduct(_ productDefinition: StoreProductDefinition) {
        if let requestedProduct = _validProducts[productDefinition.storeSpecificId] {
            print("\(#function): PurchaseProduct: \(requestedProduct.productIdentifier)")
            if canMakePayments {
                let payment = SKMutablePayment(product: requestedProduct)

                // Modify payment request for testing ask-to-buy.
                if _simulateAskToBuyEnabled {
                    print("\(#function): Queueing payment request with simulatesAskToBuyInSandbox enabled")
                    payment.simulatesAskToBuyInSandbox = true
                }

                _queue.add(payment)
            } else {
                print("\(#function): PurchaseProduct: IAP Disabled")
                onPurchaseFailed(productDefinition.storeSpecificId,
                                 "PurchasingUnavailable",
                                 "SKErrorPaymentNotAllowed",
                                 "User is not authorized to make payments")
            }
        } else {
            onPurchaseFailed(productDefinition.storeSpecificId,
                             "ItemUnavailable",
                             "",
                             "Unity IAP could not find requested product")
        }
    }

    /// Initiate a request to Apple to restore previously made purchases.
    func restorePurchases() {
        print("\(#function): RestorePurchases")
        _queue.restoreCompletedTransactions()
    }

    /// A transaction observer should be added at startup (by managed code)
    /// and maintained for the life of the app, since transactions can
    /// be delivered at any time.
    func addTransactionObserver() {
        // Detect whether an existing transaction observer is in place.
        // An existing observer will have processed any transactions already pending,
        // so when we add our own storekit will not call our updatedTransactions handler.
        // We workaround this by explicitly processing any existing transactions if they exist.
        let processExistingTransactions = _queue.transactions.count > 0

        _queue.add(self)
        if processExistingTransactions {
            paymentQueue(_queue, updatedTransactions: _queue.transactions)
        }
    }

    /// Store Kit returns a response from an SKProductsRequest.
    internal func productsRequest(_ request: SKProductsRequest, didReceive response: SKProductsResponse) {
        print("\(#function): Received \(response.products.count) products")
        // Add the retrieved products to our set of valid products.
        let fetchedProducts = Dictionary(uniqueKeysWithValues: zip(response.products.map { $0.productIdentifier },
                                                                   response.products))
        fetchedProducts.forEach { _validProducts[$0] = $1 }
        let productJson = serializeProductMetadata(response.products)

        unitySendMessage("OnProductsRetrieved",
                         productJson,
                         getAppReceipt())
    }

    /// A product metadata retrieval request failed.
    /// We handle it by retrying at an exponentially increasing interval.
    func request(_ request: SKRequest, didFailWithError error: Error) {
        _delayInSeconds = min(60, 2 * _delayInSeconds)
        print("""
        \(#function): SKProductRequest::didFailWithError: \(error.localizedDescription).
        Unity Purchasing will retry in \(_delayInSeconds) seconds
        """)
        initiateProductPoll(_delayInSeconds)
    }

    func requestDidFinish(_ request: SKRequest) {
        _request = nil
    }

    func onPurchaseFailed(_ productId: String,
                          _ reason: String,
                          _ errorCode: String,
                          _ errorDescription: String) {
        unitySendMessage("OnPurchaseFailed", EEJsonUtils.convertDictionary(toString: [
            "productId": productId,
            "reason": reason,
            "storeSpecificErrorCode": errorCode,
            "message": errorDescription
        ]))
    }

    func purchaseErrorCodeToReason(_ errorCode: Int) -> String {
        switch errorCode {
        case SKError.paymentCancelled.rawValue: return "UserCancelled"
        case SKError.paymentInvalid.rawValue: return "PaymentDeclined"
        case SKError.paymentNotAllowed.rawValue: return "PurchasingUnavailable"
        default: return "Unknown"
        }
    }

    func paymentQueue(_ queue: SKPaymentQueue, updatedTransactions transactions: [SKPaymentTransaction]) {
        print("\(#function): UpdatedTransactions")
        for transaction in transactions {
            let productIdentifier = transaction.payment.productIdentifier
            if _validProducts[productIdentifier] == nil {
                // FIXME: workaround to fix confliction with soomla.
                print("\(#function): Ignore invalid product \(productIdentifier)")
                continue
            }

            switch transaction.transactionState {
            case SKPaymentTransactionState.purchasing:
                // Item is still in the process of being purchased.
                break
            case SKPaymentTransactionState.purchased:
                onTransactionSucceeded(transaction)
            case SKPaymentTransactionState.restored:
                onTransactionSucceeded(transaction)
            case SKPaymentTransactionState.deferred:
                print("\(#function): PurchaseDeferred")
                unitySendMessage("onProductPurchaseDeferred", productIdentifier)
            case SKPaymentTransactionState.failed:
                // Purchase was either cancelled by user or an error occurred.
                let errorCode = (transaction.error as NSError?)?.code ?? -1
                print("\(#function): PurchaseFailed: \(errorCode)")

                let reason = purchaseErrorCodeToReason(errorCode)
                let errorCodeString = storeKitErrorCodeNames[errorCode] ?? "SKErrorUnknown"
                let errorDescription = "APPLE_\(transaction.error?.localizedDescription ?? "")"
                onPurchaseFailed(productIdentifier,
                                 reason,
                                 errorCodeString,
                                 errorDescription)

                // Finished transactions should be removed from the payment queue.
                _queue.finishTransaction(transaction)
                break
            @unknown default:
                fatalError()
            }
        }
    }

    /// Called when one or more transactions have been removed from the queue.
    func paymentQueue(_ queue: SKPaymentQueue, removedTransactions transactions: [SKPaymentTransaction]) {
        // Nothing to do here.
    }

    /// Called when SKPaymentQueue has finished sending restored transactions.
    func paymentQueueRestoreCompletedTransactionsFinished(_ queue: SKPaymentQueue) {
        print("\(#function): PaymentQueueRestoreCompletedTransactionsFinished")
        unitySendMessage("onTransactionsRestoredSuccess", "")
    }

    /// Called if an error occurred while restoring transactions.
    func paymentQueue(_ queue: SKPaymentQueue, restoreCompletedTransactionsFailedWithError error: Error) {
        print("\(#function): restoreCompletedTransactionsFailedWithError")
        // Restore was cancelled or an error occurred, so notify user.
        unitySendMessage("onTransactionsRestoredFail", error.localizedDescription)
    }

    func decodeProductDefinition(_ hash: [String: Any]) -> StoreProductDefinition {
        let product = StoreProductDefinition(hash["id"] as? String ?? "",
                                             hash["storeSpecificId"] as? String ?? "",
                                             hash["type"] as? Int ?? 0)
        return product
    }

    func deserializeProductDefinitions(_ json: String) -> [StoreProductDefinition] {
        let hashes = EEJsonUtils.convertString(toArray: json) as? [[String: Any]] ?? []
        var result: [StoreProductDefinition] = []
        for hash in hashes {
            result.append(decodeProductDefinition(hash))
        }
        return result
    }

    func deserializeProductDefinition(_ json: String) -> StoreProductDefinition {
        let hash = EEJsonUtils.convertString(toDictionary: json) as? [String: Any] ?? [:]
        return decodeProductDefinition(hash)
    }

    func serializeProductMetadata(_ appleProducts: [SKProduct]) -> String {
        var hashes: [[String: Any]] = []
        for product in appleProducts {
            var hash: [String: Any] = [:]
            hash["storeSpecificId"] = product.productIdentifier

            var metadata: [String: Any] = [:]
            metadata["localizedPrice"] = product.price
            metadata["isoCurrencyCode"] = product.priceLocale.currencyCode
            if let introductoryPrice = product.introductoryPrice {
                metadata["introductoryPrice"] = introductoryPrice.price
                metadata["introductoryPriceLocale"] = introductoryPrice.priceLocale.currencyCode
                metadata["introductoryPriceNumberOfPeriods"] = introductoryPrice.numberOfPeriods
                metadata["numberOfUnits"] = introductoryPrice.subscriptionPeriod.numberOfUnits
                metadata["unit"] = introductoryPrice.subscriptionPeriod.unit
            } else {
                metadata["introductoryPrice"] = ""
                metadata["introductoryPriceLocale"] = ""
                metadata["introductoryPriceNumberOfPeriods"] = ""
                metadata["numberOfUnits"] = ""
                metadata["unit"] = ""
            }
            if let subscriptionPeriod = product.subscriptionPeriod {
                metadata["subscriptionNumberOfUnits"] = subscriptionPeriod.numberOfUnits
                metadata["subscriptionPeriodUnit"] = subscriptionPeriod.unit.rawValue
            } else {
                metadata["subscriptionNumberOfUnits"] = ""
                metadata["subscriptionPeriodUnit"] = ""
            }
            let numberFormatter = NumberFormatter()
            numberFormatter.formatterBehavior = NumberFormatter.Behavior.behavior10_4
            numberFormatter.numberStyle = NumberFormatter.Style.currency
            numberFormatter.locale = product.priceLocale
            if let formattedString = numberFormatter.string(from: product.price) {
                metadata["localizedPriceString"] = formattedString
            } else {
                print("\(#function): Unable to format a localized price")
                metadata["localizedPriceString"] = ""
            }
            metadata["localizedTitle"] = product.localizedTitle
            metadata["localizedDescription"] = product.localizedDescription

            hash["metadata"] = metadata
            hashes.append(hash)
        }
        return EEJsonUtils.convertArray(toString: hashes)
    }

    private var storeKitErrorCodeNames: [Int: String] {
        return [
            SKError.unknown.rawValue: "SKErrorUnknown",
            SKError.clientInvalid.rawValue: "SKErrorClientInvalid",
            SKError.paymentCancelled.rawValue: "SKErrorPaymentCancelled",
            SKError.paymentInvalid.rawValue: "SKErrorPaymentInvalid",
            SKError.paymentNotAllowed.rawValue: "SKErrorPaymentNotAllowed",
            SKError.storeProductNotAvailable.rawValue: "SKErrorStoreProductNotAvailable",
            SKError.cloudServicePermissionDenied.rawValue: "SKErrorCloudServicePermissionDenied",
            SKError.cloudServiceNetworkConnectionFailed.rawValue: "SKErrorCloudServiceNetworkConnectionFailed",
            SKError.cloudServiceRevoked.rawValue: "SKErrorCloudServiceRevoked"
        ]
    }
}
