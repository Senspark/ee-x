//
//  Thread.swift
//  Pods
//
//  Created by eps on 6/17/20.
//

import Foundation

@objc(EEThread)
public class Thread: NSObject {
    @objc
    public class func isMainThread() -> Bool {
        return Foundation.Thread.isMainThread
    }

    @objc
    public class func runOnMainThread(_ callback: @escaping () -> Void) -> Bool {
        if isMainThread() {
            callback()
            return true
        }
        DispatchQueue.main.async {
            callback()
        }
        return false
    }

    @objc
    public class func runOnMainThreadDelayed(_ seconds: Float,
                                             _ callback: @escaping () -> Void) {
        DispatchQueue.main.asyncAfter(
            deadline: .now() + .milliseconds(Int(seconds * 1000))) {
            callback()
        }
    }
}

@_cdecl("ee_isMainThread")
public func ee_isMainThread() -> Bool {
    return Thread.isMainThread()
}

@_cdecl("ee_runOnMainThread")
public func ee_runOnMainThread() -> Bool {
    return Thread.runOnMainThread {
        ee_runOnMainThreadCallback()
    }
}

@_cdecl("ee_runOnMainThreadDelayed")
public func ee_runOnMainThreadDelayed(_ key: Int32, delay: Float) {
    Thread.runOnMainThreadDelayed(delay) {
        ee_runOnMainThreadDelayedCallback(key)
    }
}
