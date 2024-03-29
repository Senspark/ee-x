//
//  PluginManager.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/12/20.
//

import Foundation

private typealias PluginExecutor = (_ plugin: IPlugin) -> Bool

public class PluginManager: NSObject {
    private static let _sharedInstance = PluginManager()
    
    private let _logger = Logger("ee-x")
    private let _bridge = MessageBridge()
    private var _plugins: [String: IPlugin]
    
    #if os(iOS)
    private var _delegate: UIApplicationDelegate?
    #endif // os(iOS)
    
    public class var instance: PluginManager {
        return _sharedInstance
    }
    
    override private init() {
        _plugins = [:]
    }
    
    public var bridge: IMessageBridge {
        return _bridge
    }
    
    public var logger: ILogger {
        return _logger
    }
    
    #if os(iOS)
    fileprivate func initializePlugins(_ version: String, _ delegate: UIApplicationDelegate) -> Bool {
        let expectedVersion = "2.10.3"
        if version != expectedVersion {
            _logger.error("Version mismatched: found \(version) expected \(expectedVersion)")
            assert(false)
            return false
        }
        _logger.info("Initialize ee-x plugin version \(version)")
        _delegate = delegate
        swizzle(#selector(UIApplicationDelegate.application(_:open:options:)),
                #selector(PluginManager.application(_:open:options:)))
        swizzle(#selector(UIApplicationDelegate.application(_:open:sourceApplication:annotation:)),
                #selector(PluginManager.application(_:open:sourceApplication:annotation:)))
        swizzle(#selector(UIApplicationDelegate.application(_:continue:restorationHandler:)),
                #selector(PluginManager.application(_:continue:restorationHandler:)))
        Platform.registerHandlers(_bridge)
        return true
    }
    #else // os(iOS)
    fileprivate func initializePlugins() -> Bool {
        Platform.registerHandlers(_bridge)
        return true
    }
    #endif // os(iOS)
    
    fileprivate func setLogLevel(_ level: Int) {
        if let logLevel = LogLevel(rawValue: level) {
            _logger.logLevel = logLevel
        }
    }
    
    /// Adds and initialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    fileprivate func addPlugin(_ name: String) -> Bool {
        if _plugins.contains(where: { key, _ -> Bool in key == name }) {
            assert(false, "Plugin already exists: \(name)")
            return false
        }
        guard let plugin =
            // Swift plugins.
            (NSClassFromString("EE\(name)Bridge") as? IPlugin.Type)?.init(_bridge, _logger) ??
            // Objective-C plugins.
            (NSClassFromString("EE\(name)") as? NSObject.Type)?.init() as? IPlugin else {
            assert(false, "Invalid plugin: \(name)")
            return false
        }
        _plugins[name] = plugin
        return true
    }
    
    /// Removes and deinitialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    fileprivate func removePlugin(_ name: String) -> Bool {
        guard let plugin = _plugins[name] else {
            assert(false, "Plugin not exists: \(name)")
            return false
        }
        plugin.destroy()
        _plugins.removeValue(forKey: name)
        return true
    }
    
    private func executePlugins(_ executor: PluginExecutor) -> Bool {
        var response = false
        for entry in _plugins {
            if executor(entry.value) {
                response = true
            }
        }
        return response
    }
    
    #if os(iOS)
    private func swizzle(_ originalSelector: Selector, _ replacedSelector: Selector) {
        guard let replacedMethod = class_getInstanceMethod(PluginManager.self, replacedSelector) else {
            assert(false, "Invalid method")
            return
        }
        let originalClass: AnyClass? = object_getClass(_delegate)
        if let originalMethod = class_getInstanceMethod(originalClass, originalSelector) {
            method_exchangeImplementations(originalMethod, replacedMethod)
        } else {
            class_addMethod(originalClass, originalSelector,
                            method_getImplementation(replacedMethod), method_getTypeEncoding(replacedMethod))
            
            // https://stackoverflow.com/questions/28211973/swift-closure-as-anyobject/32454665
            let block: (AnyObject) -> Bool = { _ in false }
            let impl = imp_implementationWithBlock(unsafeBitCast(block as @convention(block) (AnyObject) -> Bool, to: AnyObject.self))
            method_setImplementation(replacedMethod, impl)
        }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             open url: URL,
                             options: [UIApplication.OpenURLOptionsKey: Any]) -> Bool {
        // https://stackoverflow.com/questions/39003876/calling-original-function-from-swizzled-function
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, URL, [UIApplication.OpenURLOptionsKey: Any]) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:open:options:)), Func.self)
        var response = closure(self, #selector(UIApplicationDelegate.application(_:open:options:)),
                               application, url, options)
        if (PluginManager.instance.executePlugins { $0.application?(application,
                                                                    open: url,
                                                                    options: options) ?? false }) {
            response = true
        }
        return response
    }
    
    @objc
    private func application(_ application: UIApplication,
                             open url: URL,
                             sourceApplication: String?,
                             annotation: Any) -> Bool {
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, URL, String?, Any) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:open:sourceApplication:annotation:)), Func.self)
        var response = closure(self, #selector(UIApplicationDelegate.application(_:open:sourceApplication:annotation:)),
                               application, url, sourceApplication, annotation)
        if (PluginManager.instance.executePlugins { $0.application?(application,
                                                                    open: url,
                                                                    sourceApplication: sourceApplication,
                                                                    annotation: annotation) ?? false }) {
            response = true
        }
        return response
    }
    
    @objc
    private func application(_ application: UIApplication,
                             continue userActivity: NSUserActivity,
                             restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, NSUserActivity, @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:continue:restorationHandler:)), Func.self)
        var response = closure(self, #selector(UIApplicationDelegate.application(_:continue:restorationHandler:)),
                               application, userActivity, restorationHandler)
        if (PluginManager.instance.executePlugins { $0.application?(application,
                                                                    continue: userActivity,
                                                                    restorationHandler: restorationHandler) ?? false }) {
            response = true
        }
        return response
    }
    #endif // os(iOS)
    
    fileprivate class func staticAddPlugin(_ name: UnsafePointer<CChar>) -> Bool {
        let name_str = String(cString: name)
        return _sharedInstance.addPlugin(name_str)
    }
    
    fileprivate class func staticRemovePlugin(_ name: UnsafePointer<CChar>) -> Bool {
        let name_str = String(cString: name)
        return _sharedInstance.removePlugin(name_str)
    }
}

private func ee_closureCast<T>(_ swizzledSelector: Selector,
                               _ type: T.Type) -> T {
    let impl = class_getMethodImplementation(PluginManager.self, swizzledSelector)
    return unsafeBitCast(impl, to: T.self)
}

@_cdecl("ee_staticInitializePlugins")
public func ee_staticInitializePlugins(_ version: UnsafePointer<CChar>) -> Bool {
    let version_str = String(cString: version)
    #if os(iOS)
    guard let delegate = UIApplication.shared.delegate else {
        assert(false, "Delegate not assigned")
        return false
    }
    return PluginManager.instance.initializePlugins(version_str, delegate)
    #else // os(iOS)
    return PluginManager.instance.initializePlugins(version_str)
    #endif // os(iOS)
}

@_cdecl("ee_staticSetLogLevel")
public func ee_staticSetLogLevel(_ level: Int) {
    PluginManager.instance.setLogLevel(level)
}

@_cdecl("ee_staticGetActivity")
public func ee_staticGetActivity() -> Any? {
    return nil
}

@_cdecl("ee_staticSetActivity")
public func ee_staticSetActivity(_ activity: UnsafeRawPointer) {
    // No-op.
}

@_cdecl("ee_staticAddPlugin")
public func ee_staticAddPlugin(_ name: UnsafePointer<CChar>) -> Bool {
    return PluginManager.staticAddPlugin(name)
}

@_cdecl("ee_staticRemovePlugin")
public func ee_staticRemovePlugin(_ name: UnsafePointer<CChar>) -> Bool {
    return PluginManager.staticRemovePlugin(name)
}
