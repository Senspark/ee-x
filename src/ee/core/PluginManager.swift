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
    
    private let _bridge = MessageBridge.getInstance()
    private var _plugins: [String: IPlugin]
    private var _delegate: UIApplicationDelegate?
    
    @objc
    public class func getInstance() -> PluginManager {
        return _sharedInstance
    }
    
    override private init() {
        _plugins = [:]
    }
    
    @objc
    public func initializePlugins(_ delegate: UIApplicationDelegate) -> Bool {
        _delegate = delegate
        swizzle(#selector(UIApplicationDelegate.application(_:open:options:)),
                #selector(PluginManager.application(_:open:options:)))
        swizzle(#selector(UIApplicationDelegate.application(_:open:sourceApplication:annotation:)),
                #selector(PluginManager.application(_:open:sourceApplication:annotation:)))
        swizzle(#selector(UIApplicationDelegate.application(_:continue:restorationHandler:)),
                #selector(PluginManager.application(_:continue:restorationHandler:)))
        Utils.registerHandlers(_bridge)
        return true
    }
    
    /// Adds and initialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    @objc
    public func addPlugin(_ name: String) -> Bool {
        if _plugins.contains(where: { key, _ -> Bool in key == name }) {
            assert(false, "Plugin already exists: \(name)")
            return false
        }
        guard let clazz = NSClassFromString("EE" + name) as? NSObject.Type else {
            assert(false, "Invalid plugin: \(name)")
            return false
        }
        var plugin: IPlugin?
        if name == "Store" || name == "Tenjin" {
            plugin = (clazz as! IPlugin.Type).init(_bridge)
        } else {
            plugin = clazz.init() as? IPlugin
        }
        assert(plugin != nil, "Initialization failed")
        _plugins[name] = plugin
        return false
    }
    
    /// Removes and deinitialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    @objc
    public func removePlugin(_ name: String) -> Bool {
        guard let plugin = _plugins[name] else {
            assert(false, "Plugin not exists: \(name)")
            return false
        }
        plugin.destroy()
        _plugins.removeValue(forKey: name)
        return true
    }
    
    private func executePlugins(_ executor: PluginExecutor) -> Bool {
        for entry in _plugins {
            if executor(entry.value) {
                return true
            }
        }
        return false
    }
    
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
        }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             open url: URL,
                             options: [UIApplication.OpenURLOptionsKey: Any]) -> Bool {
        // https://stackoverflow.com/questions/39003876/calling-original-function-from-swizzled-function
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, URL, [UIApplication.OpenURLOptionsKey: Any]) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:open:options:)), Func.self)
        let response = closure(self, #selector(UIApplicationDelegate.application(_:open:options:)),
                               application, url, options)
        return response || PluginManager.getInstance().executePlugins { $0.application?(application,
                                                                                        open: url,
                                                                                        options: options) ?? false }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             open url: URL,
                             sourceApplication: String?,
                             annotation: Any) -> Bool {
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, URL, String?, Any) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:open:sourceApplication:annotation:)), Func.self)
        let response = closure(self, #selector(UIApplicationDelegate.application(_:open:sourceApplication:annotation:)),
                               application, url, sourceApplication, annotation)
        return response || PluginManager.getInstance().executePlugins { $0.application?(application,
                                                                                        open: url,
                                                                                        sourceApplication: sourceApplication,
                                                                                        annotation: annotation) ?? false }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             continue userActivity: NSUserActivity,
                             restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
        typealias Func = @convention(c) (AnyObject, Selector, UIApplication, NSUserActivity, @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool
        let closure = ee_closureCast(#selector(PluginManager.application(_:continue:restorationHandler:)), Func.self)
        let response = closure(self, #selector(UIApplicationDelegate.application(_:continue:restorationHandler:)),
                               application, userActivity, restorationHandler)
        return response || PluginManager.getInstance().executePlugins { $0.application?(application,
                                                                                        continue: userActivity,
                                                                                        restorationHandler: restorationHandler) ?? false }
    }
    
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
public func ee_staticInitializePlugins() -> Bool {
    guard let delegate = UIApplication.shared.delegate else {
        assert(false, "Delegate not assigned")
        return false
    }
    return PluginManager.getInstance().initializePlugins(delegate)
}

@_cdecl("ee_staticAddPlugin")
public func ee_staticAddPlugin(_ name: UnsafePointer<CChar>) -> Bool {
    return PluginManager.staticAddPlugin(name)
}

@_cdecl("ee_staticRemovePlugin")
public func ee_staticRemovePlugin(_ name: UnsafePointer<CChar>) -> Bool {
    return PluginManager.staticRemovePlugin(name)
}
