//
//  StoreProductDefinition.swift
//  Pods
//
//  Created by eps on 6/29/20.
//

import Foundation

internal class StoreProductDefinition {
    private let _id: String
    private let _storeSpecificId: String
    private let _type: Int

    init(_ id: String, _ storeSpecificId: String, _ type: Int) {
        _id = id
        _storeSpecificId = storeSpecificId
        _type = type
    }

    var id: String {
        return _id
    }

    var storeSpecificId: String {
        return _storeSpecificId
    }

    var type: Int {
        return _type
    }
}
