//
//  IPlugin.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/12/20.
//

import Foundation

@objc(EEIPlugin)
public protocol IPlugin: NSObjectProtocol {
    init(_ bridge: IMessageBridge)

    /// Destroys this plugin.
    func destroy()

    @objc
    optional func application(_ application: UIApplication,
                              open url: URL,
                              options: [UIApplication.OpenURLOptionsKey: Any]) -> Bool
    @objc
    optional func application(_ application: UIApplication,
                              open url: URL,
                              sourceApplication: String?,
                              annotation: Any) -> Bool

    @objc
    optional func application(_ application: UIApplication,
                              continue userActivity: NSUserActivity,
                              restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool
}
