//
//  Logger.swift
//  Pods
//
//  Created by eps on 6/25/20.
//

import Foundation

internal class Logger: ILogger {
    private let _tag: String
    private var _logLevel: LogLevel

    init(_ tag: String, _ logLevel: LogLevel = LogLevel.Debug) {
        _tag = tag
        _logLevel = logLevel
    }

    var logLevel: LogLevel {
        get { _logLevel }
        set(value) { _logLevel = value }
    }

    func error(_ message: String) {
        if _logLevel.rawValue <= LogLevel.Error.rawValue {
            print("\(_tag): \(message)")
        }
    }

    func warn(_ message: String) {
        if _logLevel.rawValue <= LogLevel.Warn.rawValue {
            print("\(_tag): \(message)")
        }
    }

    func info(_ message: String) {
        if _logLevel.rawValue <= LogLevel.Info.rawValue {
            print("\(_tag): \(message)")
        }
    }

    func debug(_ message: String) {
        if _logLevel.rawValue <= LogLevel.Debug.rawValue {
            print("\(_tag): \(message)")
        }
    }

    func verbose(_ message: String) {
        if _logLevel.rawValue <= LogLevel.Verbose.rawValue {
            print("\(_tag): \(message)")
        }
    }
}
