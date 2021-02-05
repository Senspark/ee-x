//
//  PlatformUtils.swift
//  Pods
//
//  Created by eps on 6/16/20.
//

import Foundation
import Reachability
import RxSwift

public class Platform: NSObject {
    private static let kPrefix = "Platform_"

    private static let kIsApplicationInstalled = kPrefix + "isApplicationInstalled"
    private static let kOpenApplication = kPrefix + "openApplication"
    private static let kGetApplicationId = kPrefix + "getApplicationId"
    private static let kGetApplicationName = kPrefix + "getApplicationName"
    private static let kGetVersionName = kPrefix + "getVersionName"
    private static let kGetVersionCode = kPrefix + "getVersionCode"
    private static let kIsTablet = kPrefix + "isTablet"
    private static let kGetDensity = kPrefix + "getDensity"
    private static let kGetViewSize = kPrefix + "getViewSize"
    private static let kGetScreenSize = kPrefix + "getScreenSize"
    private static let kGetDeviceId = kPrefix + "getDeviceId"
    private static let kSendMail = kPrefix + "sendMail"
    private static let kTestConnection = kPrefix + "testConnection"

    public class func registerHandlers(_ bridge: IMessageBridge) {
        bridge.registerHandler(kIsApplicationInstalled) { message in
            let applicationId = message
            return Utils.toString(isApplicationInstalled(applicationId))
        }
        bridge.registerHandler(kOpenApplication) { message in
            let applicationId = message
            return Utils.toString(openApplication(applicationId))
        }
        bridge.registerHandler(kGetApplicationId) { _ in getApplicationId() }
        bridge.registerHandler(kGetApplicationName) { _ in getApplicationName() }
        bridge.registerHandler(kGetVersionName) { _ in getVersionName() }
        bridge.registerHandler(kGetVersionCode) { _ in getVersionCode() }
        bridge.registerHandler(kIsTablet) { _ in Utils.toString(isTablet()) }
        bridge.registerHandler(kGetDensity) { _ in "\(getDensity())" }
        bridge.registerHandler(kGetViewSize) { _ in
            let size = self.getViewSize()
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        bridge.registerHandler(kGetScreenSize) { _ in
            let size = self.getScreenSize()
            return EEJsonUtils.convertDictionary(toString: [
                "width": size.width,
                "height": size.height
            ])
        }
        bridge.registerAsyncHandler(kGetDeviceId) { _, resolver in
            resolver(getDeviceId())
        }
        bridge.registerHandler(kSendMail) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let recipient = dict["recipient"] as? String,
                let subject = dict["subject"] as? String,
                let body = dict["body"] as? String
            else {
                assert(false, "Unexpected value")
                return ""
            }
            return Utils.toString(sendMail(recipient, subject, body))
        }
        bridge.registerAsyncHandler(kTestConnection) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let hostName = dict["host_name"] as? String,
                let timeOut = dict["time_out"] as? Float
            else {
                assert(false, "Unexpected value")
                return
            }
            testConnection(hostName, timeOut)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
    }

    private class func isApplicationInstalled(_ applicationId: String) -> Bool {
        #if os(iOS)
        if let url = URL(string: applicationId + "://") {
            return UIApplication.shared.canOpenURL(url)
        }
        #else
        if NSWorkspace.shared.fullPath(forApplication: applicationId) != nil {
            return true
        }
        #endif
        return false
    }

    private class func openApplication(_ applicationId: String) -> Bool {
        if let url = URL(string: applicationId + "://") {
            #if os(iOS)
            return UIApplication.shared.openURL(url)
            #elseif os(OSX)
            return NSWorkspace.shared.open(url)
            #endif
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
        #if os(iOS)
        return UIDevice.current.model.hasPrefix("iPad")
        #else
        return false
        #endif
    }

    @objc
    public class func getDensity() -> Float {
        #if os(iOS)
        return Float(UIScreen.main.scale)
        #else
        let screen = NSScreen.screens.max { lhs, rhs in
            lhs.backingScaleFactor < rhs.backingScaleFactor
        }
        return Float(screen?.backingScaleFactor ?? 1)
        #endif
    }

    public class func getViewSize() -> CGSize {
        return getScreenSize()
    }

    public class func getScreenSize() -> CGSize {
        let scale = getDensity()
        let size = UIScreen.main.bounds.size
        return CGSize(width: size.width * CGFloat(scale), height: size.height * CGFloat(scale))
    }

    private class func getDeviceId() -> String {
        #if os(iOS)
        return UIDevice.current.identifierForVendor?.uuidString ?? ""
        #else
        // Get the platform expert
        let platformExpert: io_service_t = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOPlatformExpertDevice"))

        // Get the serial number as a CFString ( actually as Unmanaged<AnyObject>! )
        let serialNumberAsCFString = IORegistryEntryCreateCFProperty(
            platformExpert, kIOPlatformSerialNumberKey as CFString, kCFAllocatorDefault, 0)

        // Release the platform expert (we're responsible)
        IOObjectRelease(platformExpert)

        // Take the unretained value of the unmanaged-any-object
        // (so we're not responsible for releasing it)
        // and pass it back as a String or, if it fails, an empty string
        return (serialNumberAsCFString?.takeUnretainedValue() as? String) ?? ""
        #endif
    }

    private class func sendMail(_ recipient: String, _ subject: String, _ body: String) -> Bool {
        let str = String("mailto:\(recipient)?subject=\(subject)&body=\(body)")
        guard let encodedStr = str.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed) else {
            return false
        }
        guard let url = URL(string: encodedStr) else {
            return false
        }
        #if os(iOS)
        return UIApplication.shared.openURL(url)
        #else
        return NSWorkspace.shared.open(url)
        #endif
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
