//
//  ILogger.swift
//  Pods
//
//  Created by eps on 10/6/20.
//

import Foundation

public protocol ILogger {
    func error(_ message: String)
    func warn(_ message: String)
    func debug(_ message: String)
    func info(_ message: String)
}
