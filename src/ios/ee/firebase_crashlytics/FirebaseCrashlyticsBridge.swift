//
//  FirebaseCrashlyticsBridge.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

private let kPrefix = "FirebaseCrashlyticsBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kLog = "\(kPrefix)Log"

@objc(EEFirebaseCrashlyticsBridge)
class FirebaseCrashlyticsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private var _plugin: Crashlytics?
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _plugin = nil
        super.init()
        registerHandlers()
    }
    
    public func destroy() {
        deregisterHandlers()
    }
    
    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { _, resolver in
            resolver(Utils.toString(self.initialize()))
        }
        _bridge.registerHandler(kLog) { message in
            self.log(message)
            return ""
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kLog)
    }
    
    func initialize() -> Bool {
        if FirebaseInitializer.instance.initialize() {
            _plugin = Crashlytics.crashlytics()
            return true
        }
        return false
    }
    
    func log(_ message: String) {
        Thread.runOnMainThread {
            guard let plugin = self._plugin else {
                assert(false, "Please call initialize() first")
                return
            }
            plugin.log(message)
        }
    }
}
