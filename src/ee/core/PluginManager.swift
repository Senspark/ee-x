//
//  PluginManager.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/12/20.
//

import Foundation

private typealias PluginExecutor = (_ plugin: IPlugin) -> Bool

@objc(EEPluginManager)
public class PluginManager: NSObject {
    private static let _sharedInstance = PluginManager()
    
    private let _bridge = MessageBridge.getInstance()
    private let _plugins: [String: IPlugin]
    
    @objc
    public class func getInstance() -> PluginManager {
        return _sharedInstance
    }
    
    override private init() {
        _plugins = [:]
    }
    
    @objc
    public func initializePlugins() {
        swizzle(#selector(UIApplicationDelegate.application(_:open:options:)),
                #selector(PluginManager.application(_:open:options:)))
        swizzle(#selector(UIApplicationDelegate.application(_:open:sourceApplication:annotation:)),
                #selector(PluginManager.application(_:open:sourceApplication:annotation:)))
        swizzle(#selector(UIApplicationDelegate.application(_:continue:restorationHandler:)),
                #selector(PluginManager.application(_:continue:restorationHandler:)))
    }
    
    /// Adds and initialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    @objc
    public func addPlugin(_ name: String) {}
    
    /// Removes and deinitialize a plugin.
    /// @param[in] name The plugin's name, e.g. AdMob, Vungle.
    @objc
    public func removePlugin(_ name: String) {}
    
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
        let originalClass: AnyClass? = object_getClass(UIApplication.shared.delegate)
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
        return executePlugins { plugin in
            plugin.application(application, open: url, options: options)
        }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             open url: URL,
                             sourceApplication: String?,
                             annotation: Any) -> Bool {
        return executePlugins { plugin in
            plugin.application(application, open: url, sourceApplication: sourceApplication, annotation: annotation)
        }
    }
    
    @objc
    private func application(_ application: UIApplication,
                             continue userActivity: NSUserActivity,
                             restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
        return executePlugins { plugin in
            plugin.application(application, continue: userActivity, restorationHandler: restorationHandler)
        }
    }
}
