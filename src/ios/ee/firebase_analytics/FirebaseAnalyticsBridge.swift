import RxSwift

private let kPrefix = "FirebaseAnalyticsBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetUserProperty = "\(kPrefix)SetUserProperty"
private let kTrackScreen = "\(kPrefix)TrackScreen"
private let kLogEvent = "\(kPrefix)LogEvent"

@objc(EEFirebaseAnalyticsBridge)
class FirebaseAnalyticsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
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
        _bridge.registerHandler(kSetUserProperty) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let key = dict["key"] as? String,
                let value = dict["value"] as? String
            else {
                assert(false, "Unexpected value")
                return ""
            }
            self.setUserProperty(key, value)
            return ""
        }
        _bridge.registerHandler(kTrackScreen) { message in
            self.trackScreen(message)
            return ""
        }
        _bridge.registerHandler(kLogEvent) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let name = dict["name"] as? String,
                let parameters = dict["parameters"] as? [String: Any]
            else {
                assert(false, "Unexpected value")
                return ""
            }
            self.logEvent(name, parameters)
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetUserProperty)
        _bridge.deregisterHandler(kTrackScreen)
        _bridge.deregisterHandler(kLogEvent)
    }

    func initialize() -> Bool {
        return FirebaseInitializer.instance.initialize()
    }

    func setUserProperty(_ key: String, _ value: String) {
        Thread.runOnMainThread {
            Analytics.setUserProperty(value, forName: key)
        }
    }

    func trackScreen(_ name: String) {
        Thread.runOnMainThread {
            Analytics.logEvent(AnalyticsEventScreenView, parameters: [AnalyticsParameterScreenName: name])
        }
    }

    func logEvent(_ name: String, _ parameters: [String: Any]) {
        Thread.runOnMainThread {
            Analytics.logEvent(name, parameters: parameters)
        }
    }
}
