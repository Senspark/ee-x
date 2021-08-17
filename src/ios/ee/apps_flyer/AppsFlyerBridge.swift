//
//  AppsFlyerBridge.swift
//  ee-x-6914a733
//
//  Created by eps on 6/25/20.
//

import AppsFlyerLib

private let kTag = "\(AppsFlyerBridge.self)"
private let kPrefix = "AppsFlyerBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kStartTracking = "\(kPrefix)StartTracking"
private let kGetDeviceId = "\(kPrefix)GetDeviceId"
private let kSetDebugEnabled = "\(kPrefix)SetDebugEnabled"
private let kSetStopTracking = "\(kPrefix)SetStopTracking"
private let kTrackEvent = "\(kPrefix)TrackEvent"

@objc(EEAppsFlyerBridge)
class AppsFlyerBridge: NSObject, IPlugin, AppsFlyerLibDelegate {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _tracker = AppsFlyerLib.shared()

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
        Thread.runOnMainThread {
            self._tracker.delegate = nil
        }
    }

    func registerHandlers() {
        _bridge.registerHandler(kInitialize) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let devKey = dict["devKey"] as? String,
                let iosAppId = dict["iosAppId"] as? String
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.intialize(devKey, iosAppId)
            return ""
        }
        _bridge.registerHandler(kStartTracking) { _ in
            self.startTracking()
            return ""
        }
        _bridge.registerHandler(kGetDeviceId) { _ in
            self.deviceId
        }
        _bridge.registerHandler(kSetDebugEnabled) { message in
            self.setDebugEnabled(Utils.toBool(message))
            return ""
        }
        _bridge.registerHandler(kSetStopTracking) { message in
            self.setStopTracking(Utils.toBool(message))
            return ""
        }
        _bridge.registerHandler(kTrackEvent) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let name = dict["name"] as? String,
                let values = dict["values"] as? [String: Any]
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.trackEvent(name, values)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kStartTracking)
        _bridge.deregisterHandler(kGetDeviceId)
        _bridge.deregisterHandler(kSetDebugEnabled)
        _bridge.deregisterHandler(kSetStopTracking)
        _bridge.deregisterHandler(kTrackEvent)
    }

    func intialize(_ devKey: String, _ iosAppId: String) {
        Thread.runOnMainThread {
            self._tracker.appsFlyerDevKey = devKey
            self._tracker.appleAppID = iosAppId
            self._tracker.delegate = self
            self._tracker.shouldCollectDeviceName = true
            self._tracker.anonymizeUser = false
        }
    }

    func startTracking() {
        Thread.runOnMainThread {
            self._tracker.start()
        }
    }

    var deviceId: String {
        return _tracker.getAppsFlyerUID()
    }

    func setDebugEnabled(_ enabled: Bool) {
        Thread.runOnMainThread {
            self._tracker.isDebug = enabled
        }
    }

    func setStopTracking(_ enabled: Bool) {
        Thread.runOnMainThread {
            self._tracker.isStopped = enabled
        }
    }

    func trackEvent(_ name: String, _ values: [String: Any]) {
        Thread.runOnMainThread {
            self._tracker.logEvent(name, withValues: values)
        }
    }

    public func onConversionDataSuccess(_ conversionInfo: [AnyHashable: Any]) {
        _logger.debug("\(kTag): \(#function): \(conversionInfo)")
    }

    public func onConversionDataFail(_ error: Error) {
        _logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
    }

    public func onAppOpenAttribution(_ attributionData: [AnyHashable: Any]) {
        _logger.debug("\(kTag): \(#function): \(attributionData)")
    }

    public func onAppOpenAttributionFailure(_ error: Error) {
        _logger.debug("\(kTag): \(#function): \(error.localizedDescription)")
    }

    public func application(_ application: UIApplication, open url: URL, options: [UIApplication.OpenURLOptionsKey: Any]) -> Bool {
        _tracker.handleOpen(url, options: options)
        return false
    }

    public func application(_ application: UIApplication, open url: URL, sourceApplication: String?, annotation: Any) -> Bool {
        _tracker.handleOpen(url, sourceApplication: sourceApplication, withAnnotation: annotation)
        return false
    }

    public func application(_ application: UIApplication, continue userActivity: NSUserActivity, restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
        _tracker.continue(userActivity) { items in
            restorationHandler(items as? [UIUserActivityRestoring])
        }
        return false
    }
}
