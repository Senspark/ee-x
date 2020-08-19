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
                let logLevel = dict["logLevel"] as? Int
            else {
                assert(false, "Invalid argument")
                return ""
            }
            let _environment = environment == 0 ? ADJEnvironmentSandbox : ADJEnvironmentProduction
            let _logLevel = ADJLogLevel(rawValue: UInt32(logLevel))
            self.initialize(token, _environment, _logLevel)
            return ""
        }
        _bridge.registerHandler(kSetEnabled) { message in
            self.setEnabled(Utils.toBool(message))
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
        _bridge.deregisterHandler(kTrackEvent)
    }
    
    func initialize(_ token: String, _ environment: String, _ logLevel: ADJLogLevel) {
        let config = ADJConfig(appToken: token, environment: environment)
        config?.logLevel = logLevel
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
