//
//  FirebaseEventParameter.swift
//  ee-x-a16b5188
//
//  Created by eps on 2/1/21.
//

enum FirebaseEventType: Int {
    case Long
    case Double
    case String
}

internal class FirebaseEventParameter {
    private let _type: FirebaseEventType
    private let _value: String

    public init(_ type: Int, _ value: String) {
        _type = FirebaseEventType(rawValue: type) ?? FirebaseEventType.Long
        _value = value
    }

    public var longValue: Int64 {
        return Int64(_value) ?? 0
    }

    public var doubleValue: Double {
        return Double(_value) ?? 0
    }

    public var stringValue: String {
        return _value
    }
}
