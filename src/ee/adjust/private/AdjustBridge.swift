//
//  AdjustBridge.swift
//  Adjust
//
//  Created by eps on 8/19/20.
//

import Adjust

private let kPrefix = "AdjustBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetEnabled = "\(kPrefix)SetEnabled"
private let kGetAdvertisingIdentifier = "\(kPrefix)GetAdvertisingIdentifier"
private let kGetDeviceIdentifier = "\(kPrefix)GetDeviceIdentifier"
private let kSetPushToken = "\(kPrefix)SetPushToken"
private let kTrackEvent = "\(kPrefix)TrackEvent"

@objc(EEAdjustBridge)
public class AdjustBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    
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
            guard
                let token = dict["token"] as? String,
                let environment = dict["environment"] as? Int,
                let logLevel = dict["logLevel"] as? Int,
                let useAppSecret = dict["useAppSecret"] as? Bool,
                let secretId = dict["secretId"] as? UInt64,
                let info1 = dict["info1"] as? UInt64,
                let info2 = dict["info2"] as? UInt64,
                let info3 = dict["info3"] as? UInt64,
                let info4 = dict["info4"] as? UInt64
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.initialize(
                token, self.parseEnvironment(environment), self.parseLogLevel(logLevel),
                useAppSecret, secretId, info1, info2, info3, info4)
            return ""
        }
        _bridge.registerHandler(kSetEnabled) { message in
            self.setEnabled(Utils.toBool(message))
            return ""
        }
        _bridge.registerAsyncHandler(kGetAdvertisingIdentifier) { _, resolver in
            resolver(Adjust.adid() ?? "")
        }
        _bridge.registerHandler(kGetDeviceIdentifier) { _ in
            Adjust.adid() ?? ""
        }
        _bridge.registerHandler(kSetPushToken) { message in
            Adjust.setPushToken(message)
            return ""
        }
        _bridge.registerHandler(kTrackEvent) { message in
            self.trackEvent(message)
            return ""
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetEnabled)
        _bridge.deregisterHandler(kGetAdvertisingIdentifier)
        _bridge.deregisterHandler(kGetDeviceIdentifier)
        _bridge.deregisterHandler(kSetPushToken)
        _bridge.deregisterHandler(kTrackEvent)
    }
    
    func parseEnvironment(_ environment: Int) -> String {
        if environment == 0 {
            return ADJEnvironmentSandbox
        }
        if environment == 1 {
            return ADJEnvironmentProduction
        }
        assert(false, "Invalid argument")
        return ""
    }
    
    func parseLogLevel(_ logLevel: Int) -> ADJLogLevel {
        let dict = [
            0: ADJLogLevelVerbose,
            1: ADJLogLevelDebug,
            2: ADJLogLevelInfo,
            3: ADJLogLevelWarn,
            4: ADJLogLevelError,
            5: ADJLogLevelAssert,
            6: ADJLogLevelSuppress,
        ]
        return dict[logLevel] ?? ADJLogLevelVerbose
    }
    
    func initialize(_ token: String, _ environment: String, _ logLevel: ADJLogLevel,
                    _ useAppSecret: Bool, _ secretId: UInt64, _ info1: UInt64, _ info2: UInt64, _ info3: UInt64, _ info4: UInt64) {
        let config = ADJConfig(appToken: token, environment: environment)
        config?.logLevel = logLevel
        if useAppSecret {
            config?.setAppSecret(
                UInt(secretId),
                info1: UInt(info1),
                info2: UInt(info2),
                info3: UInt(info3),
                info4: UInt(info4))
        }
        Adjust.appDidLaunch(config)
    }
    
    func setEnabled(_ enabled: Bool) {
        Adjust.setEnabled(enabled)
    }
    
    func trackEvent(_ token: String) {
        let event = ADJEvent(eventToken: token)
        Adjust.trackEvent(event)
    }
}
