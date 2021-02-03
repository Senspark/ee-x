//
//  FirebaseCoreBridge.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

private let kTag = "\(FirebaseInitializer.self)"

class FirebaseInitializer {
    private static let _sharedInstance = FirebaseInitializer()

    private let _logger = PluginManager.instance.logger
    private var _initialized = false

    public class var instance: FirebaseInitializer {
        return _sharedInstance
    }

    private init() {
        _logger.info("\(kTag): constructor")
    }

    public func initialize() -> Bool {
        if _initialized {
            return true
        }
        _initialized = true
        FirebaseApp.configure()
        return true
    }
}
