//
//  FirebaseRemoteConfigBridge.swift
//  ee-x-a16b5188
//
//  Created by eps on 2/2/21.
//

import RxSwift

private let kPrefix = "FirebaseRemoteConfigBridge"
private let kInitialize = "\(kPrefix)Initialize"
private let kSetSettings = "\(kPrefix)SetSettings"
private let kSetDefaults = "\(kPrefix)SetDefaults"
private let kFetch = "\(kPrefix)Fetch"
private let kActivate = "\(kPrefix)Activate"
private let kGetBool = "\(kPrefix)GetBool"
private let kGetLong = "\(kPrefix)GetLong"
private let kGetDouble = "\(kPrefix)GetDouble"
private let kGetString = "\(kPrefix)GetString"

@objc(EEFirebaseRemoteConfigBridge)
class FirebaseRemoteConfigBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private var _plugin: RemoteConfig?
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        _plugin = nil
        super.init()
        registerHandlers()
    }
    
    public func destroy() {
        deregisterHandlers()
    }
    
    func registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { _, resolver in
            self.initialize()
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerAsyncHandler(kFetch) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let fetchInterval = dict["fetchInterval"] as? Int64 else {
                assert(false, "Unexpected value")
                return
            }
            self.fetch(fetchInterval)
                .subscribe(
                    onSuccess: {
                        result in resolver("\(result)")
                    }, onError: {
                        _ in resolver("\(RemoteConfigFetchStatus.failure)")
                    })
        }
        _bridge.registerAsyncHandler(kActivate) { _, resolver in
            self.activate()
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerAsyncHandler(kSetDefaults) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let defaults = dict["defaults"] as? [String: NSObject] else {
                assert(false, "Unexpected value")
                return
            }
            self.setDefaults(defaults)
                .subscribe(
                    onSuccess: {
                        _ in resolver("")
                    }, onError: {
                        _ in resolver("")
                    })
        }
        _bridge.registerHandler(kGetBool) { message in
            Utils.toString(self.getBool(message))
        }
        _bridge.registerHandler(kGetLong) { message in
            "\(self.getLong(message))"
        }
        _bridge.registerHandler(kGetDouble) { message in
            "\(self.getDouble(message))"
        }
        _bridge.registerHandler(kGetString) { message in
            self.getString(message)
        }
    }
    
    func deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetSettings)
        _bridge.deregisterHandler(kSetDefaults)
        _bridge.deregisterHandler(kFetch)
        _bridge.deregisterHandler(kActivate)
        _bridge.deregisterHandler(kGetBool)
        _bridge.deregisterHandler(kGetLong)
        _bridge.deregisterHandler(kGetDouble)
        _bridge.deregisterHandler(kGetString)
    }
    
    func initialize() -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                if FirebaseInitializer.instance.initialize() {
                    // OK.
                } else {
                    single(.success(false))
                }
                RemoteConfig.remoteConfig().ensureInitialized { _ in
                    Thread.runOnMainThread {
                        self._plugin = RemoteConfig.remoteConfig()
                        single(.success(true))
                    }
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func setSettings(_ fetchTimeOut: Int64, _ fetchInterval: Int64) -> Single<Void> {
        return Single<Void>.create { single in
            Thread.runOnMainThread {
                guard let plugin = self._plugin else {
                    assert(false, "Please call initialize() first")
                    return
                }
                let settings = RemoteConfigSettings()
                settings.fetchTimeout = TimeInterval(fetchTimeOut)
                settings.minimumFetchInterval = TimeInterval(fetchInterval)
                plugin.configSettings = settings
                single(.success(Void()))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func fetch(_ fetchInterval: Int64) -> Single<Int> {
        return Single<Int>.create { single in
            Thread.runOnMainThread {
                guard let plugin = self._plugin else {
                    assert(false, "Please call initialize() first")
                    return
                }
                plugin.fetch { status, _ in
                    Thread.runOnMainThread {
                        switch status {
                        case .success: single(.success(0))
                        case .noFetchYet: single(.success(1))
                        case .failure: single(.success(2))
                        case .throttled: single(.success(3))
                        @unknown default:
                            single(.success(2))
                        }
                    }
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func activate() -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                guard let plugin = self._plugin else {
                    assert(false, "Please call initialize() first")
                    return
                }
                plugin.activate { changed, _ in
                    Thread.runOnMainThread {
                        single(.success(changed))
                    }
                }
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func setDefaults(_ defaults: [String: NSObject]) -> Single<Void> {
        return Single<Void>.create { single in
            Thread.runOnMainThread {
                guard let plugin = self._plugin else {
                    assert(false, "Please call initialize() first")
                    return
                }
                plugin.setDefaults(defaults)
                single(.success(Void()))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func getBool(_ key: String) -> Bool {
        guard let plugin = _plugin else {
            assert(false, "Please call initialize() first")
            return false
        }
        let value = plugin.configValue(forKey: key)
        return value.boolValue
    }
    
    func getLong(_ key: String) -> Int64 {
        guard let plugin = _plugin else {
            assert(false, "Please call initialize() first")
            return 0
        }
        let value = plugin.configValue(forKey: key)
        return value.numberValue.int64Value
    }
    
    func getDouble(_ key: String) -> Double {
        guard let plugin = _plugin else {
            assert(false, "Please call initialize() first")
            return 0
        }
        let value = plugin.configValue(forKey: key)
        return value.numberValue.doubleValue
    }
    
    func getString(_ key: String) -> String {
        guard let plugin = _plugin else {
            assert(false, "Please call initialize() first")
            return ""
        }
        let value = plugin.configValue(forKey: key)
        return value.stringValue ?? ""
    }
}
