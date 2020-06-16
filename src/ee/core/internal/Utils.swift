//
//  Utils.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/3/20.
//

import Foundation

@objc(EEUtils)
public class Utils: NSObject {
    private static let kPrefix = "Utils_"
    private static let kIsMainThread = kPrefix + "isMainThread"
    private static let kRunOnUiThread = kPrefix + "runOnUiThread"
    private static let kRunOnUiThreadDelayed = kPrefix + "runOnUiThreadDelayed"
    private static let kRunOnUiThreadCallback = kPrefix + "runOnUiThreadCallback"

    #if os(iOS)
    @objc
    public class func getCurrentRootViewController() -> UIViewController? {
        return UIApplication.shared.keyWindow?.rootViewController
    }

    @objc
    public class func isLandscape() -> Bool {
        return UIApplication.shared.statusBarOrientation.isLandscape
    }
    #endif // os(iOS)

    @objc
    public class func toString(_ value: Bool) -> String {
        return value ? "true" : "false"
    }

    @objc
    public class func toBool(_ value: String) -> Bool {
        assert(value == "true" || value == "false", "Unexpected value: " + value)
        return value == "true"
    }

    @objc
    public class func registerHandlers(_ bridge: IMessageBridge) {
        bridge.registerHandler(kIsMainThread) { _ in
            toString(isMainThread())
        }
        bridge.registerHandler(kRunOnUiThread) { _ in
            toString(runOnMainThread {
                bridge.callCpp(kRunOnUiThreadCallback)
            })
        }
        bridge.registerAsyncHandler(kRunOnUiThreadDelayed) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let delay = dict["delay"] as? Float else {
                assert(false, "Unexpected value")
                return
            }
            runOnMainThreadDelayed(delay) {
                resolver("")
            }
        }
    }

    @objc
    public class func isMainThread() -> Bool {
        return Thread.isMainThread
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

    @objc
    public class func convertDpToPixels(_ dp: Float) -> Float {
        return dp * Platform.getDensity()
    }

    @objc
    public class func convertPixelsToDp(_ pixels: Float) -> Float {
        return pixels / Platform.getDensity()
    }
}

@_cdecl("ee_staticLog")
public func ee_staticLog(_ message: UnsafePointer<CChar>) {
    print(String(cString: message))
}
