//
//  Logger.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import Foundation

internal class Logger {
    private let _tag: String

    init(_ tag: String) {
        _tag = tag
    }

    func debug(_ message: String) {
        print("\(_tag): \(message)")
    }
}
