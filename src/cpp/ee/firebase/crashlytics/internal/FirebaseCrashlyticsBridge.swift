//
//  FirebaseCrashlyticsBridge.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

private let kPrefix = "FirebaseCrashlyticsBridge"
private let kLog = "\(kPrefix)Log"

@objc(EEFirebaseCrashlyticsBridge)
public class FirebaseCrashlyticsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _crashlytics = Crashlytics.crashlytics()
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        super.init()
        registerHandlers()
    }
    
    public func destroy() {
        deregisterHandlers()
    }
    
    func registerHandlers() {
        _bridge.registerHandler(kLog) { message in
            self.log(message)
            return ""
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kLog)
    }
    
    func log(_ message: String) {
        _crashlytics.log(message)
    }
}
