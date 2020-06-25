//
//  AppsFlyerBridge.swift
//  ee-x-6914a733
//
//  Created by eps on 6/25/20.
//

import AppsFlyerLib

private let kPrefix = "AppsFlyerBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kStartTracking = "\(kPrefix)StartTracking"
private let kGetDeviceId = "\(kPrefix)GetDeviceId"
private let kSetDebugEnabled = "\(kPrefix)SetDebugEnabled"
private let kSetStopTracking = "\(kPrefix)SetStopTracking"
private let kTrackEvent = "\(kPrefix)TrackEvent"

@objc(EEAppsFlyerBridge)
public class AppsFlyerBridge: NSObject, IPlugin, AppsFlyerTrackerDelegate {
    private let _logger = Logger("\(AppsFlyerBridge.self)")
    private let _bridge: IMessageBridge
    private let _tracker = AppsFlyerTracker.shared()

    public required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
    }

    func registerHandlers() {
        _bridge.registerHandler(kInitialize) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let devKey = dict["devKey"] as? String else {
                assert(false, "Invalid argument")
            }
            guard let appId = dict["appId"] as? String else {
                assert(false, "Invalid argument")
            }
            self.intialize(devKey, appId)
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
            guard let name = dict["name"] as? String else {
                assert(false, "Invalid argument")
            }
            guard let values = dict["values"] as? [String: Any] else {
                assert(false, "Invalid argument")
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

    func intialize(_ devKey: String, _ appId: String) {
        Thread.runOnMainThread {
            self._tracker.appsFlyerDevKey = devKey
            self._tracker.appleAppID = appId
            self._tracker.delegate = self
            self._tracker.shouldCollectDeviceName = true
            self._tracker.deviceTrackingDisabled = false
        }
    }

    func startTracking() {
        Thread.runOnMainThread {
            self._tracker.trackAppLaunch()
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
            self._tracker.isStopTracking = enabled
        }
    }

    func trackEvent(_ name: String, _ values: [String: Any]) {
        Thread.runOnMainThread {
            self._tracker.trackEvent(name, withValues: values)
        }
    }

    public func onConversionDataSuccess(_ conversionInfo: [AnyHashable: Any]) {
        _logger.debug("\(#function): \(conversionInfo)")
    }

    public func onConversionDataFail(_ error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
    }

    public func onAppOpenAttribution(_ attributionData: [AnyHashable: Any]) {
        _logger.debug("\(#function): \(attributionData)")
    }

    public func onAppOpenAttributionFailure(_ error: Error) {
        _logger.debug("\(#function): \(error.localizedDescription)")
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
