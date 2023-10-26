import RxSwift

private let kPrefix = "CostCenterAnalyticsBridge"
private let kInitialize = "\(kPrefix)Initialize"

@objc(EECostCenterAnalyticsBridge)
class CostCenterAnalyticsBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
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
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    func initialize() -> Bool {
        return true
    }
}
