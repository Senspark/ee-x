//
//  Utils.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/3/20.
//

import Foundation

@objc private class Utils: NSObject {
    private static let k__isMainThread = "Utils_isMainThread"
    private static let k__runOnUiThread = "Utils_runOnUiThread"
    private static let k__runOnUiThreadDelayed = "Utils_runOnUiThreadDelayed"
    private static let k__runOnUiThreadCallback = "Utils_runOnUiThreadCallback"
    private static let k__isApplicationInstalled = "Utils_isApplicationInstalled"
    private static let k__openApplication = "Utils_openApplication"
    private static let k__getApplicationId = "Utils_getApplicationId"
    private static let k__getApplicationName = "Utils_getApplicationName"
    private static let k__getVersionName = "Utils_getVersionName"
    private static let k__getVersionCode = "Utils_getVersionCode"
    private static let k__isTablet = "Utils_isTablet"
    private static let k__getDensity = "Utils_getDensity"
    private static let k__getDeviceId = "Utils_getDeviceId"
    private static let k__sendMail = "Utils_sendMail"
    private static let k__testConnection = "Utils_testConnection"

    private class func registerHandlers(_ bridge: EEIMessageBridge) {
        /*
                bridge.registerHandler(
                    k__isMainThread,
                    callback: { _ in
                        self.toString(self.isMainThread())
                           })
                bridge?.registerHandler(
                    k__runOnUiThread,
                    callback: { _ in
                        self.toString(self.run(onMainThread: {
                            self.signalMainThread()
                               }))
                           })

                bridge.registerAsyncHandler(
                    k__runOnUiThreadDelayed,
                    callback: { message, resolver in
                        let dict = EEJsonUtils.convertString(toDictionary: message)
                        let delay = (dict["delay"] as? NSNumber)?.floatValue
                        self.run(
                            onMainThreadDelayed: delay,
                            callback: {
                                resolver("")
                            })
                    })

                bridge.registerHandler(
                    k__isApplicationInstalled,
                    callback: { message in
                        let applicationId = message
                        return self.toString(
                            self.isApplicationInstalled(applicationId ?? ""))
                    })

                bridge.registerHandler(
                    k__openApplication,
                    callback: { message in
                        let applicationId = message
                        return self.toString(self.openApplication(applicationId ?? ""))
                    })

                bridge.registerHandler(
                    k__getApplicationId,
                    callback: { _ in
                        self.getApplicationId()
                    })

                bridge.registerHandler(
                    k__getApplicationName,
                    callback: { _ in
                        self.getApplicationName()
                    })

                bridge.registerHandler(
                    k__getVersionName,
                    callback: { _ in
                        self.getVersionName()
                    })

                bridge.registerHandler(
                    k__getVersionCode,
                    callback: { _ in
                        self.getVersionCode()
                    })

                bridge.registerHandler(
                    k__isTablet,
                    callback: { _ in
                        self.toString(self.isTablet())
                    })

                bridge.registerHandler(
                    k__getDensity,
                    callback: { _ in
                        let density = self.getDensity()
                        return "\(density)"
                    })

                bridge.registerAsyncHandler(
                    k__getDeviceId,
                    callback: { _, resolver in
                        let deviceId = self.getDeviceId()
                        resolver(deviceId)
                    })

                bridge.registerHandler(
                    k__sendMail,
                    callback: { message in
                        let dict = EEJsonUtils.convertString(toDictionary: message)
                        let recipient = dict["recipient"] as? String
                        let subject = dict["subject"] as? String
                        let body = dict["body"] as? String
                        return self.toString(
                            self.sendMail(
                                recipient ?? "",
                                subject: subject ?? "",
                                body: body ?? ""))
                    })

                bridge.registerAsyncHandler(
                    k__testConnection,
                    callback: { message, resolver in
                        let dict = EEJsonUtils.convertString(toDictionary: message)

                        let hostName = dict["host_name"] as? String
                        let timeOut = (dict["time_out"] as? NSNumber)?.floatValue
                        assert(hostName != nil, "")

                        self.testConnection(hostName ?? "", timeOut: timeOut).subscribeNext(
                            { x in
                                let result = (x as? NSNumber)?.boolValue ?? false
                                resolver(self.toString(result))
                            },
                            error: { _ in
                                resolver(self.toString(false))
                        })
                })
         */
    }

    // #if TARGET_OS_IOS
    private class func getKeyWindow() -> UIWindow? {
        return UIApplication.shared.keyWindow
    }

    private class func getCurrentRootViewController() -> UIViewController? {
        return getKeyWindow()?.rootViewController
    }

    private class func isLandspace() -> Bool {
        return UIApplication.shared.statusBarOrientation.isLandscape
    }

    // #endif // TARGET_OS_IOS

    private class func toString(_ value: Bool) -> String {
        return value ? "true" : "false"
    }

    private class func toBool(_ value: String) -> Bool {
        assert(value == "true" || value == "false", "Unexpected value: " + value)
        return value == "true"
    }
}
