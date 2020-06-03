//
//  Utils.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/3/20.
//

import Foundation
import Reachability
import RxSwift

@objc public class Utils: NSObject {
    private static let kPrefix = "Utils_"
    private static let kIsMainThread = kPrefix + "isMainThread"
    private static let kRunOnUiThread = kPrefix + "runOnUiThread"
    private static let kRunOnUiThreadDelayed = kPrefix + "runOnUiThreadDelayed"
    private static let kRunOnUiThreadCallback = kPrefix + "runOnUiThreadCallback"
    private static let kIsApplicationInstalled = kPrefix + "isApplicationInstalled"
    private static let kOpenApplication = kPrefix + "openApplication"
    private static let kGetApplicationId = kPrefix + "getApplicationId"
    private static let kGetApplicationName = kPrefix + "getApplicationName"
    private static let kGetVersionName = kPrefix + "getVersionName"
    private static let kGetVersionCode = kPrefix + "getVersionCode"
    private static let kIsTablet = "isTablet"
    private static let kGetDensity = "Utils_getDensity"
    private static let kGetDeviceId = "Utils_getDeviceId"
    private static let kSendMail = "Utils_sendMail"
    private static let kTestConnection = "Utils_testConnection"

    #if os(iOS)
    private class func getCurrentRootViewController() -> UIViewController? {
        return UIApplication.shared.keyWindow?.rootViewController
    }

    private class func isLandspace() -> Bool {
        return UIApplication.shared.statusBarOrientation.isLandscape
    }
    #endif // os(iOS)

    private class func toString(_ value: Bool) -> String {
        return value ? "true" : "false"
    }

    private class func toBool(_ value: String) -> Bool {
        assert(value == "true" || value == "false", "Unexpected value: " + value)
        return value == "true"
    }

    public class func registerHandlers(_ bridge: EEIMessageBridge) {
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
        bridge.registerHandler(kIsApplicationInstalled) { message in
            let applicationId = message
            return toString(isApplicationInstalled(applicationId))
        }
        bridge.registerHandler(kOpenApplication) { message in
            let applicationId = message
            return toString(openApplication(applicationId))
        }
        bridge.registerHandler(kGetApplicationId) { _ in getApplicationId() }
        bridge.registerHandler(kGetApplicationName) { _ in getApplicationName() }
        bridge.registerHandler(kGetVersionName) { _ in getVersionName() }
        bridge.registerHandler(kGetVersionCode) { _ in getVersionCode() }
        bridge.registerHandler(kIsTablet) { _ in toString(isTablet()) }
        bridge.registerHandler(kGetDensity) { _ in "\(getDensity())" }
        bridge.registerAsyncHandler(kGetDeviceId) { _, resolver in
            resolver(getDeviceId())
        }
        bridge.registerHandler(kSendMail) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let recipient = dict["recipient"] as? String else {
                assert(false, "Unexpected value")
                return ""
            }
            guard let subject = dict["subject"] as? String else {
                assert(false, "Unexpected value")
                return ""
            }
            guard let body = dict["body"] as? String else {
                assert(false, "Unexpected value")
                return ""
            }
            return toString(sendMail(recipient, subject, body))
        }
        bridge.registerAsyncHandler(kTestConnection) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let hostName = dict["host_name"] as? String else {
                assert(false, "Unexpected value")
                return
            }
            guard let timeOut = dict["time_out"] as? Float else {
                assert(false, "Unexpected value")
                return
            }
            testConnection(hostName, timeOut)
                .subscribe(
                    onSuccess: {
                        result in resolver(toString(result))
                    }, onError: {
                        _ in resolver(toString(false))
                    })
        }
    }

    private class func isMainThread() -> Bool {
        return Thread.isMainThread
    }

    private class func runOnMainThread(_ callback: @escaping () -> Void) -> Bool {
        if isMainThread() {
            callback()
            return true
        }
        DispatchQueue.main.async {
            callback()
        }
        return false
    }

    private class func runOnMainThreadDelayed(_ seconds: Float,
                                              _ callback: @escaping () -> Void) {
        DispatchQueue.main.asyncAfter(
            deadline: .now() + .milliseconds(Int(seconds * 1000))) {
            callback()
        }
    }

    private class func isApplicationInstalled(_ applicationId: String) -> Bool {
        if let url = URL(string: applicationId + "://") {
            return UIApplication.shared.canOpenURL(url)
        }
        return false
    }

    private class func openApplication(_ applicationId: String) -> Bool {
        if let url = URL(string: applicationId + "://") {
            return UIApplication.shared.openURL(url)
        }
        return false
    }

    private class func getApplicationId() -> String {
        let key = kCFBundleIdentifierKey as String
        return Bundle.main.object(forInfoDictionaryKey: key) as? String ?? ""
    }

    private class func getApplicationName() -> String {
        let key = kCFBundleNameKey as String
        return Bundle.main.object(forInfoDictionaryKey: key) as? String ?? ""
    }

    private class func getVersionName() -> String {
        let key = "CFBundleShortVersionString"
        return Bundle.main.object(forInfoDictionaryKey: key) as? String ?? ""
    }

    private class func getVersionCode() -> String {
        let key = kCFBundleVersionKey as String
        return Bundle.main.object(forInfoDictionaryKey: key) as? String ?? ""
    }

    private class func isTablet() -> Bool {
        return UIDevice.current.model.hasPrefix("iPad")
    }

    private class func getDensity() -> Float {
        return Float(UIScreen.main.scale)

        /*
         macos.
         if ([[NSScreen mainScreen]
                 respondsToSelector:@selector(backingScaleFactor)]) {
             for (NSScreen* screen in [NSScreen screens]) {
                 CGFloat scale = [screen backingScaleFactor];
                 if (scale > density) {
                     density = scale;
                 }
             }
         }
         */
    }

    private class func convertDpToPixels(_ dp: Float) -> Float {
        return dp * getDensity()
    }

    private class func convertPixelsToDp(_ pixels: Float) -> Float {
        return pixels / getDensity()
    }

    private class func getDeviceId() -> String {
        return UIDevice.current.identifierForVendor?.uuidString ?? ""
    }

    private class func sendMail(_ recipient: String, _ subject: String, _ body: String) -> Bool {
        let str = String("mailto:" + recipient + "?subject=" + subject + "&body=" + body)
        guard let encodedStr = str.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed) else {
            return false
        }
        guard let url = URL(string: encodedStr) else {
            return false
        }
        return UIApplication.shared.openURL(url)
    }

    private class func testConnection(_ hostName: String, _ timeOut: Float) -> Single<Bool> {
        let scheduler = SerialDispatchQueueScheduler(qos: .background)
        return Single<Bool>.create { single in
            do {
                let status = try Reachability(hostname: hostName).connection
                let isReachable = status != .unavailable
                single(.success(isReachable))
            } catch let ex {
                single(.error(ex))
            }
            return Disposables.create()
        }
        .subscribeOn(scheduler)
        .timeout(.milliseconds(Int(timeOut * 1000)), scheduler: scheduler)
    }
}
