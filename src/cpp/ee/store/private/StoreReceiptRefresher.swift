//
//  StoreReceiptRefresher.swift
//  Pods
//
//  Created by eps on 6/29/20.
//

import StoreKit

internal class StoreReceiptRefresher: NSObject, SKRequestDelegate {
    private let _callback: (_ success: Bool) -> Void

    init(_ callback: @escaping (_ success: Bool) -> Void) {
        _callback = callback
        super.init()
    }

    func requestDidFinish(_ request: SKRequest) {
        _callback(true)
    }

    func request(_ request: SKRequest, didFailWithError error: Error) {
        _callback(false)
    }
}
