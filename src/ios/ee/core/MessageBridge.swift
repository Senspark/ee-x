//
//  MessageBridge.swift
//  Pods
//
//  Created by eps on 6/7/20.
//

import Foundation

public class MessageBridge: NSObject, IMessageBridge {
    private static let _sharedInstance = MessageBridge()
    
    private var _handlers: [String: MessageHandler]
    private let _handleLock = "handle_lock"
    
    override public init() {
        _handlers = [:]
        super.init()
    }
    
    public func registerHandler(_ tag: String,
                                _ handler: @escaping MessageHandler) {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        if _handlers.contains(where: { key, _ -> Bool in key == tag }) {
            assert(false, "A handler with tag \(tag) already exists")
        }
        _handlers[tag] = handler
    }
    
    public func registerAsyncHandler(_ tag: String,
                                     _ handler: @escaping AsyncMessageHandler) {
        return registerHandler(tag) { message -> String in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let callbackTag = dict["callback_tag"] as? String,
                let originalMessage = dict["message"] as? String
            else {
                assert(false, "Unexpected value")
                return ""
            }
            handler(originalMessage) { callbackMessage in
                self.callCpp(callbackTag, callbackMessage)
            }
            return ""
        }
    }
    
    public func deregisterHandler(_ tag: String) {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        if !_handlers.contains(where: { key, _ -> Bool in key == tag }) {
            assert(false, "A handler with tag \(tag) doesn't exist")
        }
        _handlers.removeValue(forKey: tag)
    }
    
    private func findHandler(_ tag: String) -> MessageHandler? {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        return _handlers[tag]
    }
    
    public func callCpp(_ tag: String) {
        callCpp(tag, "")
    }
    
    public func callCpp(_ tag: String, _ message: String) {
        ee_callCppInternal(tag, message)
    }
    
    /// Calls a handler from Objective-C with a message.
    /// @warning This method should not be called manually.
    /// @param tag The tag of the handler.
    /// @param message The message.
    /// @return Reply message from Objective-C.
    private func call(_ tag: String, _ message: String) -> String {
        guard let handler = findHandler(tag) else {
            assert(false, "A handler with tag \(tag) doesn't exist")
            return ""
        }
        return handler(message)
    }
    
    fileprivate class func staticCall(_ tag: UnsafePointer<CChar>,
                                      _ message: UnsafePointer<CChar>) -> UnsafeMutablePointer<CChar>? {
        let tag_str = String(cString: tag)
        let message_str = String(cString: message)
        let response = _sharedInstance.call(tag_str, message_str)
        let response_unmanaged = strdup(response)
        return response_unmanaged
    }
}

/// https://stackoverflow.com/questions/55941571/passing-a-swift-string-to-c
@_cdecl("ee_staticCall")
public func ee_staticCall(_ tag: UnsafePointer<CChar>,
                          _ message: UnsafePointer<CChar>) -> UnsafeMutablePointer<CChar>? {
    return MessageBridge.staticCall(tag, message)
}
