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
    private let _config: RemoteConfig
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        FirebaseApp.configure()
        _bridge = bridge
        _logger = logger
        _config = RemoteConfig.remoteConfig()
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
                        _ in resolver("")
                    }, onError: {
                        _ in resolver("")
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
        _bridge.registerAsyncHandler(kGetBool) { message, resolver in
            self.getBool(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(Utils.toString(result))
                    }, onError: {
                        _ in resolver(Utils.toString(false))
                    })
        }
        _bridge.registerAsyncHandler(kGetLong) { message, resolver in
            self.getLong(message)
                .subscribe(
                    onSuccess: {
                        result in resolver("\(result)")
                    }, onError: {
                        _ in resolver("\(0)")
                    })
        }
        _bridge.registerAsyncHandler(kGetDouble) { message, resolver in
            self.getDouble(message)
                .subscribe(
                    onSuccess: {
                        result in resolver("\(result)")
                    }, onError: {
                        _ in resolver("\(0.0)")
                    })
        }
        _bridge.registerAsyncHandler(kGetString) { message, resolver in
            self.getString(message)
                .subscribe(
                    onSuccess: {
                        result in resolver(result)
                    }, onError: {
                        _ in resolver("")
                    })
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
    
    func initialize() -> Single<Void> {
        return Single<Void>.create { single in
            Thread.runOnMainThread {
                self._config.ensureInitialized { _ in
                    Thread.runOnMainThread {
                        single(.success(Void()))
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
                let settings = RemoteConfigSettings()
                settings.fetchTimeout = TimeInterval(fetchTimeOut)
                settings.minimumFetchInterval = TimeInterval(fetchInterval)
                self._config.configSettings = settings
                single(.success(Void()))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func fetch(_ fetchInterval: Int64) -> Single<Int> {
        return Single<Int>.create { single in
            Thread.runOnMainThread {
                self._config.fetch { status, _ in
                    Thread.runOnMainThread {
                        single(.success(status.rawValue))
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
                self._config.activate { changed, _ in
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
                self._config.setDefaults(defaults)
                single(.success(Void()))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func getBool(_ key: String) -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                single(.success(self._config.configValue(forKey: key).boolValue))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func getLong(_ key: String) -> Single<Int64> {
        return Single<Int64>.create { single in
            Thread.runOnMainThread {
                single(.success(self._config.configValue(forKey: key).numberValue.int64Value))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func getDouble(_ key: String) -> Single<Double> {
        return Single<Double>.create { single in
            Thread.runOnMainThread {
                single(.success(self._config.configValue(forKey: key).numberValue.doubleValue))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
    
    func getString(_ key: String) -> Single<String> {
        return Single<String>.create { single in
            Thread.runOnMainThread {
                single(.success(self._config.configValue(forKey: key).stringValue ?? ""))
            }
            return Disposables.create()
        }
        .subscribeOn(MainScheduler())
    }
}
