//
//  AdColonyBridge.swift
//  ee-x-7d8a1a4f
//
//  Created by eps on 11/20/20.
//

import AdColony
import RxSwift

private let kPrefix = "AdColonyBridge"
private let kInitialize = "\(kPrefix)Initialize"

@objc(EEAdColony)
public class AdColonyBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _initializing = false
    private var _initialized = false

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
        _bridge.registerAsyncHandler(kInitialize) { message, resolver in
            self.initialize(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    func initialize(_ appId: String) -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                if self._initializing {
                    single(.success(false))
                    return
                }
                if self._initialized {
                    single(.success(true))
                    return
                }
                AdColony.configure(withAppID: appId, zoneIDs: [], options: nil) { _ in
                    self._initializing = false
                    self._initialized = true
                    single(.success(true))
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
}
