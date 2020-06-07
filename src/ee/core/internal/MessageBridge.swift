//
//  MessageBridge.swift
//  Pods
//
//  Created by eps on 6/7/20.
//

import Foundation

@objc(EEMessageBridge)
public class MessageBridge: NSObject, IMessageBridge {
    private static let _sharedInstance = MessageBridge()
    
    private var _handlers: [String: MessageHandler]
    private let _handleLock = "handle_lock"
    
    @objc
    public class func getInstance() -> IMessageBridge {
        return _sharedInstance
    }
    
    override private init() {
        _handlers = [String: MessageHandler]()
        super.init()
    }
    
    @discardableResult
    public func registerHandler(_ tag: String,
                                _ handler: @escaping MessageHandler) -> Bool {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        if _handlers.contains(where: { key, _ -> Bool in key == tag }) {
            assert(false, "A handler with tag " + tag + " already exists")
            return false
        }
        _handlers[tag] = handler
        return true
    }
    
    @discardableResult
    public func registerAsyncHandler(_ tag: String,
                                     _ handler: @escaping MessageAsyncHandler) -> Bool {
        return registerHandler(tag) { message -> String in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let callbackTag = dict["callback_tag"] as? String else {
                assert(false, "Unexpected value")
                return ""
            }
            guard let originalMessage = dict["message"] as? String else {
                assert(false, "Unexpected value")
                return ""
            }
            handler(originalMessage) { callbackMessage in
                self.callCpp(callbackTag, callbackMessage)
            }
            return ""
        }
    }
    
    @discardableResult
    public func deregisterHandler(_ tag: String) -> Bool {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        if !_handlers.contains(where: { key, _ -> Bool in key == tag }) {
            assert(false, "A handler with tag " + tag + " doesn't exist")
            return false
        }
        _handlers.removeValue(forKey: tag)
        return true
    }
    
    private func findHandler(_ tag: String) -> MessageHandler? {
        objc_sync_enter(_handleLock)
        defer { objc_sync_exit(_handleLock) }
        return _handlers[tag]
    }
    
    public func call(_ tag: String, _ message: String) -> String {
        guard let handler = findHandler(tag) else {
            assert(false, "A handler with tag " + tag + " doesn't exist")
            return ""
        }
        return handler(message)
    }
    
    @discardableResult
    public func callCpp(_ tag: String, _ message: String) -> String {
        guard let response_cpp = ee_callCppInternal(tag, message) else {
            return ""
        }
        let response = String(cString: response_cpp)
        ee_freeString(response_cpp)
        return response
    }
}

/// https://stackoverflow.com/questions/55941571/passing-a-swift-string-to-c
@_cdecl("ee_staticCall")
public func ee_staticCall(_ tag: UnsafePointer<CChar>,
                          _ message: UnsafePointer<CChar>) -> UnsafeMutablePointer<CChar>? {
    let tag_str = String(cString: tag)
    let message_str = String(cString: message)
    let response = MessageBridge.getInstance().call(tag_str, message_str)
    let response_unmanaged = strdup(response)
    return response_unmanaged
}
