//
//  IMessageBridge.swift
//  Pods
//
//  Created by eps on 6/7/20.
//

import Foundation

public typealias MessageHandler = (_ message: String) -> String
public typealias MessageAsyncResolver = (_ message: String) -> Void
public typealias MessageAsyncHandler =
    (_ message: String, _ resolver: @escaping MessageAsyncResolver) -> Void

@objc(EEIMessageBridge)
public protocol IMessageBridge {
    /// Registers a new handler to receive messages from C++.
    /// @param tag The unique tag of the handler.
    /// @param handler The handler.
    /// @return Whether the registration was successful.
    @discardableResult
    func registerHandler(_ tag: String,
                         _ handler: @escaping MessageHandler) -> Bool

    /// Registers a new async handler to receive messages from C++.
    /// @param tag The unique tag of the handler.
    /// @param handler The handler.
    /// @return Whether the registration was successful.
    @discardableResult
    func registerAsyncHandler(_ tag: String,
                              _ handler: @escaping MessageAsyncHandler) -> Bool

    /// Deregisters an existing handler not to receive
    /// messages from C++.
    /// @param tag The unique tag of the handler.
    /// @return Whether the deregistration was successful.
    @discardableResult
    func deregisterHandler(_ tag: String) -> Bool

    /// Calls a handler from Objective-C with a message.
    /// @warning This method should not be called manually.
    /// @param tag The tag of the handler.
    /// @param message The message.
    /// @return Reply message from Objective-C.
    func call(_ tag: String, _ message: String) -> String

    /// Calls a handler from C++ without a message.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from C++.
    @discardableResult
    func callCpp(_ tag: String, _ message: String) -> String
}

extension IMessageBridge {
    @discardableResult
    func callCpp(_ tag: String) -> String {
        return callCpp(tag, "")
    }
}
