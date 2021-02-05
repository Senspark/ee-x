//
//  FirebasePerformanceBridge.swift
//  ee-x-bb8dc50f
//
//  Created by eps on 2/3/21.
//

private let kPrefix = "FirebasePerformanceBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kIsDataCollectionEnabled = "\(kPrefix)IsDataCollectionEnabled"
private let kSetDataCollectionEnabled = "\(kPrefix)SetDataCollectionEnabled"
private let kNewTrace = "\(kPrefix)NewTrace"

@objc(EEFirebasePerformanceBridge)
class FirebasePerformanceBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private var _plugin: Performance?
    private var _traces: [String: FirebasePerformanceTrace]
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _plugin = nil
        _traces = [:]
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
        _bridge.registerHandler(kIsDataCollectionEnabled) { _ in
            Utils.toString(self.isDataCollectionEnabled)
        }
        _bridge.registerHandler(kSetDataCollectionEnabled) { message in
            self.isDataCollectionEnabled = Utils.toBool(message)
            return ""
        }
        _bridge.registerHandler(kNewTrace) { message in
            Utils.toString(self.newTrace(message))
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kIsDataCollectionEnabled)
        _bridge.deregisterHandler(kSetDataCollectionEnabled)
        _bridge.deregisterHandler(kNewTrace)
    }
    
    func initialize() -> Bool {
        if FirebaseInitializer.instance.initialize() {
            _plugin = Performance.sharedInstance()
            return true
        }
        return false
    }
    
    var isDataCollectionEnabled: Bool {
        get {
            guard let plugin = _plugin else {
                assert(false, "Please call initialize() first")
                return false
            }
            return plugin.isDataCollectionEnabled
        }
        set(value) {
            guard let plugin = _plugin else {
                assert(false, "Please call initialize() first")
                return
            }
            return plugin.isDataCollectionEnabled = value
        }
    }
    
    func newTrace(_ traceName: String) -> Bool {
        guard let plugin = _plugin else {
            assert(false, "Please call initialize() first")
            return false
        }
        if _traces.contains(where: { key, _ in key == traceName }) {
            return false
        }
        guard let trace = plugin.trace(name: traceName) else {
            return false
        }
        _traces[traceName] = FirebasePerformanceTrace(_bridge, traceName, trace)
        return true
    }
}
